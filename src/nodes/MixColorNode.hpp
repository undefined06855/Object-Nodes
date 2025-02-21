#pragma once
#include "../GUINode.hpp"
#include "ccTypes.h"
#include <imgui.h>

class MixColorNode : public GuiNode {
public:
    std::string getName() const { return "MixColorNode"; }

    MixColorNode() : GuiNode(
        /* title */ "Mix Color",
        /* col */ NODE_COLOR_COLOR,
        /* inputs */ {
            NODE_PIN("", PIN_TYPE_DROPDOWN(
                "Darken", "Multiply", "Color Burn",
                "Lighten", "Screen", "Color Dodge", "Add",
                "Overlay", "Soft Light", "Linear Light",
                "Difference", "Exclusion", "Subtract", "Divide",
                "Hue", "Saturation", "Color", "Value"
            )),
            NODE_PIN("Factor", PIN_TYPE_DOUBLE+1),
            NODE_PIN("Color A", PIN_TYPE_COLOR),
            NODE_PIN("Color B", PIN_TYPE_COLOR)
        },
        /* outputs */ {
            NODE_PIN("Color", PIN_TYPE_COLOR)
        }
    ) {};

    inline float clamp(float value) {
        return std::max(0.0f, std::min(1.0f, value));
    }

    inline float mix(float a, float b, float factor) {
        return a + factor * (b - a);
    }    

    void onInputChange() {
        auto value = std::get<double>(m_inputs[1]->m_value);
        m_inputs[1]->m_value = clamp(value);
    }

    bool compute() {
        auto colorA = std::get<cocos2d::ccColor4F>(m_inputs[2]->m_value);
        auto colorB = std::get<cocos2d::ccColor4F>(m_inputs[3]->m_value);

        cocos2d::ccColor4F blended;
        switch (m_inputs[0]->m_dropDownSelectedValue) {
            case 0: { // Darken
                blended.r = std::min(colorA.r, colorB.r);
                blended.g = std::min(colorA.g, colorB.g);
                blended.b = std::min(colorA.b, colorB.b);
                break;
            }
            case 1: { // Multiply
                blended.r = colorA.r * colorB.r;
                blended.g = colorA.g * colorB.g;
                blended.b = colorA.b * colorB.b;
                break;
            }
            case 2: { // Color Burn
                blended.r = (colorB.r == 0.0f) ? 0.0f : clamp(1.0f - ((1.0f - colorA.r) / colorB.r));
                blended.g = (colorB.g == 0.0f) ? 0.0f : clamp(1.0f - ((1.0f - colorA.g) / colorB.g));
                blended.b = (colorB.b == 0.0f) ? 0.0f : clamp(1.0f - ((1.0f - colorA.b) / colorB.b));
                break;
            }
            case 3: { // Lighten
                blended.r = std::max(colorA.r, colorB.r);
                blended.g = std::max(colorA.g, colorB.g);
                blended.b = std::max(colorA.b, colorB.b);
                break;
            }
            case 4: { // Screen
                blended.r = 1.0f - (1.0f - colorA.r) * (1.0f - colorB.r);
                blended.g = 1.0f - (1.0f - colorA.g) * (1.0f - colorB.g);
                blended.b = 1.0f - (1.0f - colorA.b) * (1.0f - colorB.b);
                break;
            }
            case 5: { // Color Dodge
                blended.r = (colorB.r == 0.0f) ? 1.0f : clamp(colorA.r / (1.0f - colorB.r));
                blended.g = (colorB.g == 0.0f) ? 1.0f : clamp(colorA.g / (1.0f - colorB.g));
                blended.b = (colorB.b == 0.0f) ? 1.0f : clamp(colorA.b / (1.0f - colorB.b));
                break;
            }
            case 6: { // Add
                blended.r = clamp(colorA.r + colorB.r);
                blended.g = clamp(colorA.g + colorB.g);
                blended.b = clamp(colorA.b + colorB.b);
                break;
            }
            case 7: { // Overlay
                blended.r = (colorA.r < 0.5f) ? (2.0f * colorA.r * colorB.r) : (1.0f - 2.0f * (1.0f - colorA.r) * (1.0f - colorB.r));
                blended.g = (colorA.g < 0.5f) ? (2.0f * colorA.g * colorB.g) : (1.0f - 2.0f * (1.0f - colorA.g) * (1.0f - colorB.g));
                blended.b = (colorA.b < 0.5f) ? (2.0f * colorA.b * colorB.b) : (1.0f - 2.0f * (1.0f - colorA.b) * (1.0f - colorB.b));
                break;
            }
            case 8: { // Soft Light
                blended.r = (colorA.r < 0.5f) ? (2.0f * colorA.r * colorB.r) : (1.0f - 2.0f * (1.0f - colorA.r) * (1.0f - colorB.r));
                blended.g = (colorA.g < 0.5f) ? (2.0f * colorA.g * colorB.g) : (1.0f - 2.0f * (1.0f - colorA.g) * (1.0f - colorB.g));
                blended.b = (colorA.b < 0.5f) ? (2.0f * colorA.b * colorB.b) : (1.0f - 2.0f * (1.0f - colorA.b) * (1.0f - colorB.b));
                break;
            }
            case 9: { // Linear Light
                blended.r = (colorA.r < 0.5f) ? (colorA.r + 2.0f * colorB.r - 1.0f) : (colorA.r + 2.0f * (colorB.r - 0.5f));
                blended.g = (colorA.g < 0.5f) ? (colorA.g + 2.0f * colorB.g - 1.0f) : (colorA.g + 2.0f * (colorB.g - 0.5f));
                blended.b = (colorA.b < 0.5f) ? (colorA.b + 2.0f * colorB.b - 1.0f) : (colorA.b + 2.0f * (colorB.b - 0.5f));
                break;
            }
            case 10: { // Difference
                blended.r = std::abs(colorA.r - colorB.r);
                blended.g = std::abs(colorA.g - colorB.g);
                blended.b = std::abs(colorA.b - colorB.b);
                break;
            }
            case 11: { // Exclusion
                blended.r = colorA.r + colorB.r - 2.0f * colorA.r * colorB.r;
                blended.g = colorA.g + colorB.g - 2.0f * colorA.g * colorB.g;
                blended.b = colorA.b + colorB.b - 2.0f * colorA.b * colorB.b;
                break;
            }
            case 12: { // Subtract
                blended.r = clamp(colorA.r - colorB.r);
                blended.g = clamp(colorA.g - colorB.g);
                blended.b = clamp(colorA.b - colorB.b);
                break;
            }
            case 13: { // Divide
                blended.r = (colorB.r == 0.0f) ? 1.0f : clamp(colorA.r / colorB.r);
                blended.g = (colorB.g == 0.0f) ? 1.0f : clamp(colorA.g / colorB.g);
                blended.b = (colorB.b == 0.0f) ? 1.0f : clamp(colorA.b / colorB.b);
                break;
            }
            case 14: { // Hue
                cocos2d::ccColor4F hsvA, hsvB;
                ImGui::ColorConvertRGBtoHSV(colorA.r, colorA.g, colorA.b, hsvA.r, hsvA.g, hsvA.b);
                ImGui::ColorConvertRGBtoHSV(colorB.r, colorB.g, colorB.b, hsvB.r, hsvB.g, hsvB.b);
                hsvA.r = hsvB.r;
                ImGui::ColorConvertHSVtoRGB(hsvA.r, hsvA.g, hsvA.b, blended.r, blended.g, blended.b);
                break;
            }
            case 15: { // Saturation
                cocos2d::ccColor4F hsvA, hsvB;
                ImGui::ColorConvertRGBtoHSV(colorA.r, colorA.g, colorA.b, hsvA.r, hsvA.g, hsvA.b);
                ImGui::ColorConvertRGBtoHSV(colorB.r, colorB.g, colorB.b, hsvB.r, hsvB.g, hsvB.b);
                hsvA.g = hsvB.g;
                ImGui::ColorConvertHSVtoRGB(hsvA.r, hsvA.g, hsvA.b, blended.r, blended.g, blended.b);
                break;
            }
            case 16: { // Color
                cocos2d::ccColor4F hsvA, hsvB;
                ImGui::ColorConvertRGBtoHSV(colorA.r, colorA.g, colorA.b, hsvA.r, hsvA.g, hsvA.b);
                ImGui::ColorConvertRGBtoHSV(colorB.r, colorB.g, colorB.b, hsvB.r, hsvB.g, hsvB.b);
                hsvA.r = hsvB.r;
                hsvA.g = hsvB.g;
                ImGui::ColorConvertHSVtoRGB(hsvA.r, hsvA.g, hsvA.b, blended.r, blended.g, blended.b);
                break;
            }
            case 17: { // Value
                cocos2d::ccColor4F hsvA, hsvB;
                ImGui::ColorConvertRGBtoHSV(colorA.r, colorA.g, colorA.b, hsvA.r, hsvA.g, hsvA.b);
                ImGui::ColorConvertRGBtoHSV(colorB.r, colorB.g, colorB.b, hsvB.r, hsvB.g, hsvB.b);
                hsvA.b = hsvB.b;
                ImGui::ColorConvertHSVtoRGB(hsvA.r, hsvA.g, hsvA.b, blended.r, blended.g, blended.b);
                break;
            }
        }
        

        // the factor is actually fake and doesnt do anything to the actual
        // mixing and I did not know that until i started to do this and tested
        // it in blender
    
        auto factor = std::get<double>(m_inputs[1]->m_value);
        auto& output = std::get<cocos2d::ccColor4F>(m_outputs[0]->m_value);
        output.r = mix(colorA.r, blended.r, factor);
        output.g = mix(colorA.g, blended.g, factor);
        output.b = mix(colorA.b, blended.b, factor);
        output.a = mix(colorA.a, colorB.a, factor);

        geode::log::debug("output colour: rgba({}, {}, {}, {})", output.r, output.g, output.b, output.a);

        return true;
    }
};
