#pragma once
#include "../GUINode.hpp"
#include <imgui.h>

class ArrayNode : public GuiNode {
public:
    std::string getName() const { return "ArrayNode"; }

    ArrayNode() : GuiNode(
        /* title */ "Array",
        /* col */ NODE_COLOR_BLOCK,
        /* inputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT),
            NODE_PIN(PIN_TYPE_SPACER),
            NODE_PIN("Offset", PIN_TYPE_CCPOINT + ccp(30.f, 0.f)),
            NODE_PIN("Count", PIN_TYPE_DOUBLE + 2, 1.f),
        },
        /* outputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT)
        }
    ) {};

    void onInputChange() {
        m_inputs[3]->m_value = std::floor(std::get<double>(m_inputs[3]->m_value));
    }

    bool compute() {
        auto repeatCount = std::get<double>(m_inputs[3]->m_value);

        auto obj = std::get<sp_GameObjectData>(m_inputs[0]->m_value);
        auto offset = std::get<cocos2d::CCPoint>(m_inputs[2]->m_value);

        auto copy = std::make_shared<GameObjectData>(*obj);
        auto x = copy->propertyAs<double>(2);
        auto y = copy->propertyAs<double>(3);

        x += offset.x * m_computeIteration;
        y += offset.y * m_computeIteration;

        copy->m_objectValues[2] = std::to_string(x);
        copy->m_objectValues[3] = std::to_string(y);

        m_outputs[0]->m_value = copy;

        // check if this is final compute
        if (m_computeIteration + 1 >= repeatCount) return true;
        return false;
    }
};
