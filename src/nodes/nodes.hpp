// temp file


#include "../GUINode.hpp"
#include <imgui.h>

class TestNode : public GuiNode {
public:
    TestNode() : GuiNode(
        /* title */ "test node",
        /* col */ IM_COL32(102, 51, 153, 255),
        /* inputs */ {
            NODE_PIN("skibidi position input", PIN_TYPE_CCPOINT),
            NODE_PIN("test coloUr", PIN_TYPE_COLOR),
            NODE_PIN("triple (real not a double)", PIN_TYPE_DOUBLE),
            NODE_PIN("rattledash EATER", PIN_TYPE_GAMEOBJECT)
        },
        /* outputs */ {
            NODE_PIN("the boolean of all time", PIN_TYPE_BOOL),
            NODE_PIN("rattledash object", PIN_TYPE_GAMEOBJECT),
            NODE_PIN("test output pos", PIN_TYPE_CCPOINT)
        }
    ) {};

    void compute() {
        // do shit
        geode::log::info("compute deez nuts");
    }
};


class TestConstantsNode : public GuiNode {
public:
    TestConstantsNode() : GuiNode(
        /* title */ "input momento",
        /* col */ IM_COL32(0, 0, 0, 0),
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("haha 69", PIN_TYPE_DOUBLE),
            NODE_PIN("position", PIN_TYPE_CCPOINT)
        }
    ) {};

    void compute() {
        geode::log::info("constants");
        m_outputs[0]->m_value = 69.f;
        m_outputs[1]->m_value = cocos2d::CCPoint{ 69.f, 420.f };
    }
};

class TestMultiply : public GuiNode {
public:
    TestMultiply() : GuiNode(
        /* title */ "multiply by two node (crazy)",
        /* col */ IM_COL32(23, 150, 27, 255),
        /* inputs */ {
            NODE_PIN("value that should be multiplied", PIN_TYPE_DOUBLE)
        },
        /* outputs */ {
            NODE_PIN("value but multiplied by two", PIN_TYPE_DOUBLE)
        }
    ) {};

    void compute() {
        geode::log::info("multiplying by two... (very complex)");
        m_outputs[0]->m_value = std::get<double>(m_inputs[0]->m_value) * 2.f;
    }
};


class TestOutput : public GuiNode {
public:
    TestOutput() : GuiNode(
        /* title */ "the flalertation",
        /* col */ IM_COL32(54, 8, 115, 255),
        /* inputs */ {
            NODE_PIN("numebr", PIN_TYPE_DOUBLE)
        },
        /* outputs */ {}
    ) {};

    void compute() {
        FLAlertLayer::create(
            "output",
            fmt::format("{}", std::get<double>(m_inputs[0]->m_value)).c_str(),
            "wow"
        )->show();
    }
};

