#pragma once
#include <imnodes.h>
#include "structs.hpp"

namespace on::utils {

ImNodesPinShape pinShapeForType(PinType pinType);
unsigned int pinColorForType(PinType pinType);
bool shouldSkipPin(sp_PinData pin);

unsigned int brightenColor(unsigned int imGuiColor, int amount);

std::string cutOffImGuiString(std::string string, float maxWidth);

}
