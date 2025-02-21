#include "structs.hpp"
#include "Geode/binding/GJBaseGameLayer.hpp"
#include "NodeManager.hpp"
#include <fmt/std.h>

PinData::PinData(std::string name, PinType value)
    : m_name(name)
    , m_value(value)
    , m_increment(.1f)
    , m_showInput(true)
    , m_id(NodeManager::get().getNextPinID())
    , m_isSpacerPin(false)
    , m_dropDownSelectedValue(0) {}

PinData::PinData(std::string name, PinType value, double increment)
    : m_name(name)
    , m_value(value)
    , m_increment(increment)
    , m_showInput(true)
    , m_id(NodeManager::get().getNextPinID())
    , m_isSpacerPin(false)
    , m_dropDownSelectedValue(0) {}

PinData::PinData(std::string name, PinType value, double increment, bool showInput)
    : m_name(name)
    , m_value(value)
    , m_increment(increment)
    , m_showInput(showInput)
    , m_id(NodeManager::get().getNextPinID())
    , m_isSpacerPin(false)
    , m_dropDownSelectedValue(0) {}

PinData::PinData(bool isSpacerPin)
    : m_name("")
    , m_value(false)
    , m_increment(.1f)
    , m_showInput(true)
    , m_id(0)
    , m_isSpacerPin(true)
    , m_dropDownSelectedValue(0) {}

LinkData::LinkData(int from, int to)
    : m_from(from)
    , m_to(to) {}

GameObjectData::GameObjectData(int id)
    : m_objectValues({}) {
    // thanks to https://flowvix.github.io/gd-info-explorer/
    // these are just common properties that should probably be set

    m_objectValues[1] = id;
    m_objectValues[2] = 0.f; // x
    m_objectValues[3] = 0.f; // y
}

GameObjectData::GameObjectData(GameObject* obj)
    : m_objectValues({}) {
    auto str = obj->getSaveString(GJBaseGameLayer::get());

    int lastPropertyKey = 0;
    bool isKey = true;
    while (!str.empty()) {
        if (isKey) {
            // find the key
            auto indexOfNextComma = str.find(",");
            if (indexOfNextComma == std::string::npos) return;

            lastPropertyKey = std::stoi(str.substr(0, indexOfNextComma));
            str.erase(0, indexOfNextComma+1);
            isKey = false;
            continue;
        }

        // find the value
        auto indexOfNextComma = str.find(",");
        auto value = str.substr(0, indexOfNextComma);
        str.erase(0, indexOfNextComma+1);

        m_objectValues[lastPropertyKey] = value;

        isKey = true;
    }
}

std::string GameObjectData::commitToString() {
    std::string ret = "";
    for (auto pair : m_objectValues) {
        ret += fmt::format("{},{},", pair.first, pair.second);
    }
    return ret;
}

template <typename T>
T GameObjectData::propertyAs(int id) {
    if (m_objectValues.find(id) == m_objectValues.end()) {
        if constexpr (std::is_same_v<T, std::string>) m_objectValues[id] = "";
        else m_objectValues[id] = "0"; // zero or false
    }

    auto value = m_objectValues[id];

    if constexpr (std::is_same_v<T, std::string>) return value;
    else if constexpr (std::is_same_v<T, int>) return std::stoi(value);
    else if constexpr (std::is_same_v<T, double>) return std::stod(value);
    else if constexpr (std::is_same_v<T, bool>) return value == "1";
    else static_assert(false, "unsupported type for object property!!");
}
