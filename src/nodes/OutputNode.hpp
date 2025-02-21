#pragma once
#include "../GUINode.hpp"
#include <imgui.h>

// this is a special node - it's referred to by type multiple times
// so dont delete it ig

class OutputNode : public GuiNode {
public:
    std::string getName() const { return "OutputNode"; }

    OutputNode() : GuiNode(
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

    std::vector<sp_GameObjectData> m_objects = {};

    bool compute() {
        m_objects.push_back(std::get<sp_GameObjectData>(m_inputs[0]->m_value));
        return true;
    }
};
