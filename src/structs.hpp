#pragma once
#include <Geode/binding/GameObject.hpp>

struct GameObjectData;

using PinType = std::variant<bool, double, cocos2d::CCPoint, cocos2d::ccColor4F, std::shared_ptr<GameObjectData>, std::vector<const char*>>;

// borrowed kindly from blender, some changed slightly
#define NODE_COLOR_INPUT 0xff3371ff
#define NODE_COLOR_OUTPUT 0x4d0017ff
#define NODE_COLOR_BLOCK 0x00d6a3ff
#define NODE_COLOR_COLOR 0xcccc00ff

#define PIN_TYPE_BOOL false
#define PIN_TYPE_DOUBLE 0.0
#define PIN_TYPE_CCPOINT cocos2d::CCPoint{0, 0}
#define PIN_TYPE_COLOR cocos2d::ccColor4F{ 0, 0, 0, 1 }
#define PIN_TYPE_GAMEOBJECT std::shared_ptr<GameObjectData>(nullptr)
#define PIN_TYPE_DROPDOWN(...) std::vector<const char*>{__VA_ARGS__}
// fake!!!
#define PIN_TYPE_SPACER true

#define NODE_PIN(...) std::make_shared<PinData>(__VA_ARGS__)

struct PinData {
    std::string m_name;
    PinType m_value;

    double m_increment;
    bool m_showInput;
    
    unsigned int m_id;
    bool m_isSpacerPin;

    int m_dropDownSelectedValue;

    PinData(std::string name, PinType value);
    PinData(std::string name, PinType value, double increment);
    PinData(std::string name, PinType value, double increment, bool showInput);
    PinData(bool isSpacerPin);
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

struct GameObjectData {
    std::unordered_map<int, std::string> m_objectValues;

    GameObjectData(int id);
    GameObjectData(GameObject* obj);
    std::string commitToString();
    template <typename T>
    T propertyAs(int id);
};
using sp_GameObjectData = std::shared_ptr<GameObjectData>;
