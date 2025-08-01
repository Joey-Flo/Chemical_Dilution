#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>

class ModelListener;

class Model
{
public:

    int8_t getPumpEnableState(int8_t index);
    void setPumpEnableState(int8_t index, bool is_enabled);

    float getPumpDensity(int8_t index);
    void setPumpDensity(int8_t index, float new_density);

    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
