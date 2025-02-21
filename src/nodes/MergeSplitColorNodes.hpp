#pragma once
#include "../GUINode.hpp"
#include <imgui.h>

class MergeColorNode : public GuiNode {
public:
    std::string getName() const { return "MergeColorNode"; }

    MergeColorNode() : GuiNode(
        /* title */ "Merge Color",
        /* col */ NODE_COLOR_COLOR,
        /* inputs */ {
            NODE_PIN("", PIN_TYPE_DROPDOWN("RGB", "HSV")),
            NODE_PIN("Red", PIN_TYPE_DOUBLE),
            NODE_PIN("Green", PIN_TYPE_DOUBLE),
            NODE_PIN("Blue", PIN_TYPE_DOUBLE),
            NODE_PIN("Alpha", PIN_TYPE_DOUBLE)
        },
        /* outputs */ {
            NODE_PIN("Color", PIN_TYPE_COLOR)
        }
    ) {};

    void onInputChange() {
        switch(m_inputs[0]->m_dropDownSelectedValue) {
            case 0:
                // rgb
                m_inputs[1]->m_name = "Red";
                m_inputs[2]->m_name = "Green";
                m_inputs[3]->m_name = "Blue";
                break;
            case 1:
                // hsv
                m_inputs[1]->m_name = "Hue";
                m_inputs[2]->m_name = "Saturation";
                m_inputs[3]->m_name = "Value";
        }
    }

    bool compute() {
        float first = std::get<double>(m_inputs[1]->m_value);
        float second = std::get<double>(m_inputs[2]->m_value);
        float third = std::get<double>(m_inputs[3]->m_value);
        float alpha = std::get<double>(m_inputs[4]->m_value);
        
        switch(m_inputs[0]->m_dropDownSelectedValue) {
            case 0:
                // rgb
                m_outputs[0]->m_value = cocos2d::ccColor4F{ first, second, third, alpha };
                break;
            case 1:
                // hsv
                float r, g, b;
                ImGui::ColorConvertHSVtoRGB(first, second, third, r, g, b);
                m_outputs[0]->m_value = cocos2d::ccColor4F{ r, g, b, alpha };
        }

        return true;
    }
};

class SplitColorNode : public GuiNode {
public:
    std::string getName() const { return "SplitColorNode"; }

    SplitColorNode() : GuiNode(
        /* title */ "Split Color",
        /* col */ NODE_COLOR_COLOR,
        /* inputs */ {
            NODE_PIN("", PIN_TYPE_DROPDOWN("RGB", "HSV")),
            NODE_PIN("Color", PIN_TYPE_COLOR)
        },
        /* outputs */ {
            NODE_PIN("Red", PIN_TYPE_DOUBLE),
            NODE_PIN("Green", PIN_TYPE_DOUBLE),
            NODE_PIN("Blue", PIN_TYPE_DOUBLE),
            NODE_PIN("Alpha", PIN_TYPE_DOUBLE)
        }
    ) {};

    void onInputChange() {
        switch(m_inputs[0]->m_dropDownSelectedValue) {
            case 0:
                // rgb
                m_outputs[0]->m_name = "Red";
                m_outputs[1]->m_name = "Green";
                m_outputs[2]->m_name = "Blue";
                break;
            case 1:
                // hsv
                m_outputs[0]->m_name = "Hue";
                m_outputs[1]->m_name = "Saturation";
                m_outputs[2]->m_name = "Value";
        }
    }

    bool compute() {
        auto col = std::get<cocos2d::ccColor4F>(m_inputs[0]->m_value);

        float first, second, third;
        switch(m_inputs[0]->m_dropDownSelectedValue) {
            case 0:
                // rgb
                first = col.r;
                second = col.g;
                third = col.b;
                break;
            case 1:
                // hsv
                ImGui::ColorConvertRGBtoHSV(col.r, col.g, col.b, first, second, third);
        }

        m_outputs[0]->m_value = first;
        m_outputs[1]->m_value = second;
        m_outputs[2]->m_value = third;
        
        m_outputs[3]->m_value = (double)col.a;
        return true;
    }
};
