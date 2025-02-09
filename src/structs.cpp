#include "structs.hpp"
#include "GUIManager.hpp"

PinData::PinData(std::string name, PinType value)
    : m_name(name)
    , m_value(value)
    , m_id(GuiManager::get().getNextPinID()) {}

LinkData::LinkData(int from, int to)
    : m_from(from)
    , m_to(to) {}
