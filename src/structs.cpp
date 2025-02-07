#include "structs.hpp"

int g_lastLinkConnectionID = 0;
PinData::PinData(std::string name, PinType value)
    : m_name(name)
    , m_value(value)
    , m_id(g_lastLinkConnectionID++) {}

LinkData::LinkData(int from, int to)
    : m_from(from)
    , m_to(to) {}
