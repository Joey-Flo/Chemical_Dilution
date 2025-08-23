// In Core/Inc/DispenseManager.h

#ifndef INC_DISPENSEMANAGER_H_
#define INC_DISPENSEMANAGER_H_

#include "main.h" // This is a C-only header now
#include "DispenseManager_shared_types.h" // Include our shared types

// --- Public Function Prototypes ---
void DispenseManager_Init(void);
void DispenseManager_Process(void);
int DispenseManager_StartJob(int8_t recipe_index, int8_t size);
void DispenseManager_CancelJob(void);
const DispenseJob_t* DispenseManager_GetJobStatus(void);
float DispenseManager_GetCurrentWeight(void);

#endif /* INC_DISPENSEMANAGER_H_ */
