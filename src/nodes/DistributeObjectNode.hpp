#pragma once
#include "../GuiNode.hpp"

class DistributeObjectNode : public CollectingGuiNode {
public:
    std::string getName() const { return "DistributeObjectNode"; }

    DistributeObjectNode() : CollectingGuiNode(
        /* title */ "Distribute Objects",
        /* col */ NODE_COLOR_OUTPUT,
        /* inputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT),
            NODE_PIN("From", PIN_TYPE_CCPOINT),
            NODE_PIN("To", PIN_TYPE_CCPOINT)
        },
        /* outputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT)
        }
    ) {};

    bool compute() {
        // m_objects is provided by CollectingGuiNode
        return true;
    }
};
