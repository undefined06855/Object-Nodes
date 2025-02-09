#pragma once
#include <imnodes.h>
#include "structs.hpp"

namespace on::utils {

ImNodesPinShape pinShapeForType(PinType pinType);
unsigned int pinColorForType(PinType pinType);

unsigned int brightenColor(unsigned int imGuiColor, int amount);

cocos2d::CCNode* createHR();

}
