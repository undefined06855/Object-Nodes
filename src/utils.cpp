#include "utils.hpp"
#include <typeindex>

namespace on::utils {

ImNodesPinShape pinShapeForType(PinType pinType) {
    // get it because like a pin cushion haha funny right
    std::map<std::type_index, ImNodesPinShape> cushion = {
        { typeid(bool), ImNodesPinShape_CircleFilled },
        { typeid(double), ImNodesPinShape_CircleFilled },
        { typeid(cocos2d::CCPoint), ImNodesPinShape_CircleFilled },
        { typeid(cocos2d::ccColor4B), ImNodesPinShape_CircleFilled },
        { typeid(GameObject*), ImNodesPinShape_QuadFilled }
    };

    return std::visit([&](auto&& value) {
        return cushion.find(typeid(std::decay_t<decltype(value)>))->second;
    }, pinType);
}

unsigned int pinColorForType(PinType pinType) {
    std::map<std::type_index, unsigned int> colors = {
        { typeid(bool), IM_COL32(179, 107, 194, 255) }, // lilac
        { typeid(double), IM_COL32(128, 128, 128, 255) }, // grey
        { typeid(cocos2d::CCPoint), IM_COL32(94, 32, 201, 255) },  // purple
        { typeid(cocos2d::ccColor4B), IM_COL32(225, 232, 14, 255) }, // yellow
        { typeid(GameObject*), IM_COL32(252, 186, 3, 255) }   // orange
    };

    return std::visit([&](auto&& value) {
        return colors.find(typeid(std::decay_t<decltype(value)>))->second;
    }, pinType);
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

}
