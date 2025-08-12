#ifndef CHEMICALSSETUPPRESENTER_HPP
#define CHEMICALSSETUPPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "shared_types.h" // For PumpSetup_t

using namespace touchgfx;

class ChemicalsSetupView;

class ChemicalsSetupPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ChemicalsSetupPresenter(ChemicalsSetupView& v);

    /**
     * @brief Called when this screen is activated.
     */
    virtual void activate();

    /**
     * @brief Called when this screen is deactivated.
     */
    virtual void deactivate();

    virtual ~ChemicalsSetupPresenter() {}

    /**
     * @brief Commands the View to load and display the data for a specific recipe page.
     *        Also updates the Presenter's internal state to remember the active page.
     * @param page_index The 0-based index of the recipe to load.
     */
    void loadScreenData(int page_index);

    /**
     * @brief Saves the updated data for a single PumpSetupWidget.
     * @param page_index The index of the recipe being edited.
     * @param setup_index The index of the pump setup within the recipe (0-2).
     * @param data The new PumpSetup_t data to save.
     */
    void savePumpSetupData(int page_index, int setup_index, const PumpSetup_t& data);

    /**
     * @brief Initiates an edit operation for a specific field.
     * @param fieldID The unique ID of the field to be edited.
     */
    void editField(int fieldID);

    /**
     * @brief Processes the new text value entered by the user via the keyboard.
     * @param text The new value as a C-string.
     */
    void newValueEntered(const char* text);

    /**
     * @brief Adds a new pump to the specified recipe.
     * @param page_index The index of the recipe to modify.
     */
    void addPumpSetup(int page_index);

    /**
     * @brief Removes the last pump from the specified recipe.
     * @param page_index The index of the recipe to modify.
     */
    void removePumpSetup(int page_index);

    /**
     * @brief Toggles the enabled/disabled state of the current recipe.
     */
    void chemicalEnableToggled();

    /**
     * @brief A function for the View to explicitly update the Presenter's active page index.
     * @param index The new 0-based page index.
     */
    void ActiveFieldIndexUpdate(uint8_t index);

private:
    ChemicalsSetupPresenter();

    ChemicalsSetupView& view;

    // State variables for managing UI context
    int currentlyEditingFieldID;
    int activePageIndex;
    int activeSetupIndex;
    int activeFieldIndex;
};

#endif // CHEMICALSSETUPPRESENTER_HPP
