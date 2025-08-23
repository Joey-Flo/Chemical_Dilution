// In Core/Src/DispenseManager.c

#include "DispenseManager.h"
#include "device_config.h"
#include "hx711.h"
#include "MuxDriver.h"
#include "PWMDriver.h"
#include "ValveDriver.h"
#include <stdbool.h>
#include "Sounds.h"

// --- MACROS for fine-tuning ---
#define CONTAINER_MIN_WEIGHT_GRAMS    25.0f
#define CLOSE_THRESHOLD_PERCENT       0.80f  // Slow down at 80% of target
#define PUMP_PWM_FAST                 1000   // 100% duty cycle
#define PUMP_PWM_SLOW                 200    // 20% duty cycle
#define DRIP_SETTLE_DELAY_MS          500    // Wait 0.5s for drips after stopping a pump
#define STABLE_READING_COUNT_TARGET   12

// --- Global variables for this module ---
extern DeviceConfiguration_t myDeviceConfig;
extern hx711_t my_scale;
extern TIM_HandleTypeDef htim5; // Assumes pump PWM is on TIM5

static DispenseJob_t currentJob;
static float current_weight_grams = 0.0f;
static uint8_t stable_reading_count = 0;
static float weight_at_job_end = 0.0f;
bool DispenseCompleteSound = true;
bool DispenseStartSound = true;

// --- Helper Functions ---

/**
 * @brief Turns a specific pump ON to a given PWM value.
 */
static void Pump_On(uint8_t pump_index, uint16_t pwm_value)
{
    PumpChannelSelect(pump_index);
    DutyCycleAdjust(&htim5, TIM_CHANNEL_2, pwm_value);
}

/**
 * @brief Turns the currently selected pump OFF.
 */
static void Pump_Off(void)
{
    DutyCycleAdjust(&htim5, TIM_CHANNEL_2, 0);
}

/**
 * @brief Reads the scale and updates the global current_weight_grams.
 * @return true if a new reading was available, false otherwise.
 */
static bool Update_Scale_Reading(void)
{
    // 1. Get the raw, averaged value from the sensor. This is NOT yet tared.
    long raw_average_value = hx711_value_ave(&my_scale, 2);

    // 2. Manually subtract the offset that was stored during the tare operation.
    //    This uses the '.' operator because my_scale is a struct, not a pointer.
    long tared_value = raw_average_value - my_scale.offset;

    // 3. Convert the final, tared value to grams using the stored calibration factor.
    if (myDeviceConfig.scale_calibration_factor != 0.0f)
    {
        current_weight_grams = (float)tared_value / myDeviceConfig.scale_calibration_factor;
    }
    else
    {
        current_weight_grams = 0.0f; // Avoid division by zero if not calibrated
    }

    return true; // The function always succeeds in getting a value
}


// --- Public API Functions ---

void DispenseManager_Init(void)
{
    currentJob.state = DISPENSE_STATE_IDLE;
    stable_reading_count = 0;
    weight_at_job_end = 0.0f;
    DispenseCompleteSound = true;
    DispenseStartSound = true;
    Pump_Off();
    Valve_Close();
}

int DispenseManager_StartJob(int8_t recipe_index, int8_t size)
{
    if (currentJob.state != DISPENSE_STATE_IDLE) {
        return -1; // A job is already in progress
    }

    currentJob.recipe_index = recipe_index;
    currentJob.dispense_size = size;
    currentJob.state = DISPENSE_STATE_WAITING_FOR_CONTAINER;

    // --- THIS IS THE CORRECTED LOGIC ---

    // 1. Get the recipe we are working with.
    ChemicalRecipe_t* recipe = &myDeviceConfig.recipes[recipe_index];

    // 2. Get the recipe's total volume (which is always in mL).
    float total_volume_mL = recipe->total_dispense_volume;

    // 3. Calculate the total weight of the CHEMICALS ONLY.
    float chemical_only_weight_grams = 0.0f;
    float chemical_only_volume_mL = 0.0f;

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        PumpSetup_t* pump_setup = &recipe->pump_setups[i];
        if (pump_setup->pump_index != -1) // Check if the pump is active in this recipe
        {
            int8_t pump_index = pump_setup->pump_index;
            float pump_density = myDeviceConfig.PumpDensity[pump_index];
            float target_volume = 0.0f;

            if (size == 0) target_volume = pump_setup->dispense_small;
            else if (size == 1) target_volume = pump_setup->dispense_medium;
            else target_volume = pump_setup->dispense_large;

            // Add this chemical's contribution to the totals
            chemical_only_volume_mL += target_volume;
            chemical_only_weight_grams += (target_volume * pump_density);
        }
    }

    // 4. Calculate the volume of water needed.
    float water_volume_mL = total_volume_mL - chemical_only_volume_mL;
    if (water_volume_mL < 0) water_volume_mL = 0; // Safety check

    // 5. Calculate the weight of the water (density of water is 1.0 g/mL).
    float water_weight_grams = water_volume_mL * 1.0f;

    // 6. The final total target weight is the sum of the chemicals and the water.
    currentJob.total_target_weight_grams = chemical_only_weight_grams + water_weight_grams;

    return 0;
}

void DispenseManager_CancelJob(void)
{
    Pump_Off();
    Valve_Close();
    // --- MODIFIED: Store the current weight before changing state ---
    Update_Scale_Reading(); // Get one last reading
    weight_at_job_end = current_weight_grams;
    currentJob.state = DISPENSE_STATE_WAITING_FOR_REMOVAL;
}

const DispenseJob_t* DispenseManager_GetJobStatus(void)
{
    return &currentJob;
}

float DispenseManager_GetCurrentWeight(void)
{
    return current_weight_grams;
}

int DispenseManager_StartJob_SkipContainerCheck(int8_t recipe_index, int8_t size)
{
    if (currentJob.state != DISPENSE_STATE_IDLE) {
        return -1; // A job is already in progress
    }

    currentJob.recipe_index = recipe_index;
    currentJob.dispense_size = size;

    // --- This is the MODIFIED entry point ---
    // Instead of waiting for the container, we jump DIRECTLY to taring.
    currentJob.state = DISPENSE_STATE_TARING;
    stable_reading_count = 0; // Ensure this is reset

    // The rest of the function is identical to the original StartJob
    // to ensure the target weight is still calculated correctly.
    ChemicalRecipe_t* recipe = &myDeviceConfig.recipes[recipe_index];
    float total_volume_mL = recipe->total_dispense_volume;
    float chemical_only_weight_grams = 0.0f;
    float chemical_only_volume_mL = 0.0f;

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        PumpSetup_t* pump_setup = &recipe->pump_setups[i];
        if (pump_setup->pump_index != -1)
        {
            int8_t pump_index = pump_setup->pump_index;
            float pump_density = myDeviceConfig.PumpDensity[pump_index];
            float target_volume = 0.0f;

            if (size == 0) target_volume = pump_setup->dispense_small;
            else if (size == 1) target_volume = pump_setup->dispense_medium;
            else target_volume = pump_setup->dispense_large;

            chemical_only_volume_mL += target_volume;
            chemical_only_weight_grams += (target_volume * pump_density);
        }
    }

    float water_volume_mL = total_volume_mL - chemical_only_volume_mL;
    if (water_volume_mL < 0) water_volume_mL = 0;
    float water_weight_grams = water_volume_mL * 1.0f;
    currentJob.total_target_weight_grams = chemical_only_weight_grams + water_weight_grams;

    return 0;
}

// --- The Main State Machine ---

void DispenseManager_Process(void)
{
    // No need to call Update_Scale_Reading() here anymore.
    // We will call it only when we need a fresh reading inside the states.

    switch (currentJob.state)
    {
        // ... (IDLE, DONE, CANCELLED states are unchanged) ...

    case DISPENSE_STATE_WAITING_FOR_CONTAINER:
    {
        Update_Scale_Reading();
        if (current_weight_grams >= CONTAINER_MIN_WEIGHT_GRAMS) {
            // --- NEW: Increment stable reading counter ---
            stable_reading_count++;
        } else {
            // --- NEW: Reset counter if weight is unstable or removed ---
            stable_reading_count = 0;
        }

        // --- NEW: Only proceed if we reach the target count ---
        if (stable_reading_count >= STABLE_READING_COUNT_TARGET) {
            stable_reading_count = 0; // Reset for next time
            currentJob.state = DISPENSE_STATE_TARING;
        }
        break;
    }

        case DISPENSE_STATE_TARING:
            // This is the correct way to tare with your library
            hx711_tare(&my_scale, 10);
            current_weight_grams = 0.0f; // Reset our software value
            currentJob.current_pump_setup_index = -1;
            currentJob.state = DISPENSE_STATE_START_PUMP;
            if (DispenseStartSound == true){
            	DispenseStartSound = false;
            	PlaySound(2);
            }
            break;

        // ... (START_PUMP state is unchanged) ...
        case DISPENSE_STATE_START_PUMP:
        {
            // Find the next active pump in the recipe
            int next_pump_setup = -1;
            for (int i = currentJob.current_pump_setup_index + 1; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i) {
                if (myDeviceConfig.recipes[currentJob.recipe_index].pump_setups[i].pump_index != -1) {
                    next_pump_setup = i;
                    break;
                }
            }

            if (next_pump_setup != -1) {
                // We found the next pump to dispense.
                currentJob.current_pump_setup_index = next_pump_setup;

                // --- THIS IS THE FIX: The declarations are now inside this block ---
                PumpSetup_t* pump_setup = &myDeviceConfig.recipes[currentJob.recipe_index].pump_setups[next_pump_setup];
                int8_t pump_index = pump_setup->pump_index;
                float pump_density = myDeviceConfig.PumpDensity[pump_index];
                float target_volume = 0.0f;

                if (currentJob.dispense_size == 0) target_volume = pump_setup->dispense_small;
                else if (currentJob.dispense_size == 1) target_volume = pump_setup->dispense_medium;
                else target_volume = pump_setup->dispense_large;

                // The target weight for this chemical is its own weight PLUS the weight already in the container
                currentJob.current_target_weight_grams = current_weight_grams + (target_volume * pump_density);

                Pump_On(pump_index, PUMP_PWM_FAST);
                currentJob.state = DISPENSE_STATE_DISPENSING_FAST;
            } else {
                // No more pumps left, move on to the water fill.
                currentJob.state = DISPENSE_STATE_START_WATER;
            }
            break;
        }

        case DISPENSE_STATE_DISPENSING_FAST:
            Update_Scale_Reading(); // Get a fresh reading
            if (current_weight_grams >= (currentJob.current_target_weight_grams * CLOSE_THRESHOLD_PERCENT)) {
                int8_t pump_index = myDeviceConfig.recipes[currentJob.recipe_index].pump_setups[currentJob.current_pump_setup_index].pump_index;
                Pump_On(pump_index, PUMP_PWM_SLOW);
                currentJob.state = DISPENSE_STATE_DISPENSING_SLOW;
            }
            break;

        case DISPENSE_STATE_DISPENSING_SLOW:
            Update_Scale_Reading(); // Get a fresh reading
            if (current_weight_grams >= currentJob.current_target_weight_grams) {
                Pump_Off();
                HAL_Delay(DRIP_SETTLE_DELAY_MS);
                currentJob.state = DISPENSE_STATE_START_PUMP;
            }
            break;

        case DISPENSE_STATE_START_WATER:
            // This state is entered when all chemicals are done.
            // Open the solenoid valve to start the water fill.
            Valve_Open();

            // Immediately transition to the next state to begin monitoring the weight.
            currentJob.state = DISPENSE_STATE_WATER_FILL;
            break;
        // --- END OF ADDED BLOCK ---

        case DISPENSE_STATE_WATER_FILL:
            Update_Scale_Reading(); // Get a fresh reading
            if (current_weight_grams >= currentJob.total_target_weight_grams) {
                Valve_Close();
                currentJob.state = DISPENSE_STATE_DONE;
            }
            break;

        case DISPENSE_STATE_DONE:
            // --- MODIFIED: Store the final weight before waiting for removal ---
            weight_at_job_end = current_weight_grams;
            currentJob.state = DISPENSE_STATE_WAITING_FOR_REMOVAL;
            break;

            // --- NEW STATE: Wait for the container to be removed ---
        case DISPENSE_STATE_WAITING_FOR_REMOVAL:
            if (DispenseCompleteSound == true){
            	DispenseCompleteSound = false;
            	PlaySound(5);
            }
             Update_Scale_Reading();

             // The container is considered "removed" if the current weight
             // has dropped by at least half of the minimum container weight.
             // This works even if the current weight is negative.
             // Example: end weight is 150g. Threshold is 150 - 12.5 = 137.5g.
             // When container is removed, weight becomes -100g. -100g is less than 137.5g, so it passes.
             if (current_weight_grams < (weight_at_job_end - (CONTAINER_MIN_WEIGHT_GRAMS / 2.0f)))
             {
                 currentJob.state = DISPENSE_STATE_POST_JOB_TARE;
             }
             break;

            // --- NEW STATE: Perform the final, automatic re-tare ---
            case DISPENSE_STATE_POST_JOB_TARE:
                hx711_tare(&my_scale, 10); // Re-tare the scale to zero it out.
                current_weight_grams = 0.0f;
                // The job is now truly finished. The UI will see the IDLE state
                // and know it's time to return to the home screen.
                currentJob.state = DISPENSE_STATE_IDLE;
                break;

        case DISPENSE_STATE_IDLE:
            // The system is idle. The state machine has no work to do.
            // It is waiting for the UI to call DispenseManager_StartJob().
            break;

        case DISPENSE_STATE_CANCELLED:
            // This state is now a passthrough. If it's ever entered by other means,
            // it will be handled by the explicit call in CancelJob().
            break;
    }
}
