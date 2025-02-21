#include "utils.hpp"

namespace on::utils {

ImNodesPinShape pinShapeForType(PinType pinType) {
    // get it because like a pin cushion haha funny right
    std::array<ImNodesPinShape, 5> cushion = {
        /* bool */ ImNodesPinShape_CircleFilled,
        /* double */ ImNodesPinShape_CircleFilled,
        /* ccpoint */ ImNodesPinShape_CircleFilled,
        /* ccColor4F */ ImNodesPinShape_CircleFilled,
        /* sp_GameObjectData */ ImNodesPinShape_QuadFilled
    };

    return cushion[pinType.index()];
}

unsigned int pinColorForType(PinType pinType) {
    std::array<ImU32, 5> colors = {
        /* bool */ IM_COL32(179, 107, 194, 255), // lilac
        /* double */ IM_COL32(128, 128, 128, 255), // grey
        /* ccpoint */ IM_COL32(94, 32, 201, 255), // purple
        /* ccColor4F */ IM_COL32(225, 232, 14, 255), // yellow
        /* sp_GameObjectData */ IM_COL32(252, 186, 3, 255) // orange
    };

    return colors[pinType.index()];
}

bool shouldSkipPin(sp_PinData pin) {
    return pin->m_isSpacerPin || pin->m_value.index() == 5;
}

unsigned int brightenColor(unsigned int imGuiColor, int amount) {
    int red   = (imGuiColor >> IM_COL32_R_SHIFT) & 0xff;
    int green = (imGuiColor >> IM_COL32_G_SHIFT) & 0xff;
    int blue  = (imGuiColor >> IM_COL32_B_SHIFT) & 0xff;
    int alpha = (imGuiColor >> IM_COL32_A_SHIFT) & 0xff;

    red = std::min(255, red + amount);
    green = std::min(255, green + amount);
    blue = std::min(255, blue + amount);

    return IM_COL32(red, green, blue, alpha);
}

std::string cutOffImGuiString(std::string string, float maxWidth) {
    if (ImGui::CalcTextSize(string.c_str()).x < maxWidth) return string;

    bool changed = false;
    string += "...";
    while (ImGui::CalcTextSize(string.c_str()).x > maxWidth) {
        string = string.substr(0, string.length() - 4);
        string += "...";
        changed = true;
    }

    return string;
}

}
