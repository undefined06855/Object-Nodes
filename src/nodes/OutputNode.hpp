#pragma once
#include "../GuiNode.hpp"

class OutputNode : public CollectingGuiNode {
public:
    std::string getName() const { return "OutputNode"; }

    OutputNode() : CollectingGuiNode(
        /* title */ "Output",
        /* col */ NODE_COLOR_OUTPUT,
        /* inputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT)
        },
        /* outputs */ {}
    ) {
        m_width = 100.f;
        m_endParsingAtThisNode = true;
    };

    bool compute() {
        // m_objects is provided by CollectingGuiNode
        m_objects.push_back(std::get<sp_GameObjectData>(m_inputs[0]->m_value));
        return true;
    }
};
