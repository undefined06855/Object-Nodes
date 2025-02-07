#pragma once
#include <Geode/binding/GameObject.hpp>

using PinType = std::variant<bool, double, cocos2d::CCPoint, cocos2d::ccColor4B, GameObject*>;

#define PIN_TYPE_BOOL false
#define PIN_TYPE_DOUBLE 0.0
#define PIN_TYPE_CCPOINT cocos2d::CCPoint{0, 0}
#define PIN_TYPE_COLOR cocos2d::ccColor4B{0, 0, 0, 0}
#define PIN_TYPE_GAMEOBJECT (GameObject*)nullptr

#define NODE_PIN(...) std::make_shared<PinData>(__VA_ARGS__)

struct PinData {
    std::string m_name;
    PinType m_value;
    unsigned int m_id;

    PinData(std::string name, PinType value);
};
using sp_PinData = std::shared_ptr<PinData>;

// this used to have more in it but idk doesnt need it anymore
// kept instead of just an std::pair in case more is needed later
struct LinkData {
    int m_from;
    int m_to;

    LinkData(int from, int to);
};
using sp_LinkData = std::shared_ptr<LinkData>;
