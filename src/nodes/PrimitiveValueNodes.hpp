#pragma once
#include "../GuiNode.hpp"
#include <imgui.h>

class BooleanNode : public GuiNode {
public:
    std::string getName() const { return "BooleanNode"; }

    BooleanNode() : GuiNode(
        /* title */ "Boolean",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("Boolean", PIN_TYPE_BOOL)
        }
    ) {
        m_width = 100.f;
    };

    bool m_value = false;

    void drawExtra() {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.f);
        ImGui::SetNextItemWidth(m_width);
        bool ret = ImGui::Checkbox("", &m_value);
        if (ret) onInputChangeAndUpdatePreview();
    }

    bool compute() {
        m_outputs[0]->m_value = m_value;
        return true;
    }
};

class ValueNode : public GuiNode {
public:
    std::string getName() const { return "ValueNode"; }

    ValueNode() : GuiNode(
        /* title */ "Value",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("Value", PIN_TYPE_DOUBLE)
        }
    ) {
        m_width = 100.f;
    };

    double m_value = 0.f;

    void drawExtra() {
        double zero = 0.f;
        ImGui::SetNextItemWidth(m_width);
        bool ret = ImGui::DragScalar("", ImGuiDataType_Double, &m_value, ImGui::GetIO().KeyShift ? .01f : .1f, &zero, &zero, "%.2f");
        if (ret) onInputChangeAndUpdatePreview();
    }

    bool compute() {
        m_outputs[0]->m_value = m_value;
        return true;
    }
};

class PositionNode : public GuiNode {
public:
    std::string getName() const { return "PositionNode"; }

    PositionNode() : GuiNode(
        /* title */ "Position",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("Position", PIN_TYPE_CCPOINT)
        }
    ) {
        m_width = 100.f;
    };

    cocos2d::CCPoint m_value = { 0.f, 0.f };

    void drawExtra() {
        float temp[2] = { m_value.x, m_value.y };
        ImGui::SetNextItemWidth(m_width);
        bool ret = ImGui::DragFloat2("", temp, ImGui::GetIO().KeyShift ? .01f : .1f, 0.f, 0.f, "%.2f");
        m_value.x = temp[0];
        m_value.y = temp[1];
        if (ret) onInputChangeAndUpdatePreview();
    }

    bool compute() {
        m_outputs[0]->m_value = m_value;
        return true;
    }
};


class ColorNode : public GuiNode {
public:
    std::string getName() const { return "ColorNode"; }

    ColorNode() : GuiNode(
        /* title */ "Color",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("Color", PIN_TYPE_COLOR)
        }
    ) {
        // colour picker is pretty chunky, keep at default
        // m_width = 100.f;
    };

    cocos2d::ccColor4F m_value = { 0, 0, 0, 1 };

    void drawExtra() {
        float temp[4] = { m_value.r, m_value.g, m_value.b, m_value.a };

        ImGui::SetNextItemWidth(m_width);
        bool ret = ImGui::ColorPicker4(
            "",
            temp,
            ImGuiColorEditFlags_DisplayHex
            | ImGuiColorEditFlags_DisplayHSV
            | ImGuiColorEditFlags_DisplayRGB
            | ImGuiColorEditFlags_PickerHueBar
            | ImGuiColorEditFlags_NoBorder
            | ImGuiColorEditFlags_InputRGB
        );
        
        m_value.r = temp[0];
        m_value.g = temp[1];
        m_value.b = temp[2];
        m_value.a = temp[3];

        if (ret) onInputChangeAndUpdatePreview();
    }

    bool compute() {
        m_outputs[0]->m_value = m_value;
        return true;
    }
};
