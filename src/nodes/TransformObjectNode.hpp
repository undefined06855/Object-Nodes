// #pragma once
// #include "../GUINode.hpp"
// #include <imgui.h>

// // TODO: maybe just separate this out into separate nodes?

// class TransformObjectNode : public GuiNode {
// public:
//     std::string getName() const { return "TransformObjectNode"; }

//     TransformObjectNode() : GuiNode(
//         /* title */ "Transform Object",
//         /* col */ IM_COL32(136, 26, 232, 255),
//         /* inputs */ {
//             NODE_PIN("Object", PIN_TYPE_GAMEOBJECT),
//             NODE_PIN(PIN_TYPE_SPACER),
//             NODE_PIN("Position", PIN_TYPE_CCPOINT),
//             NODE_PIN(PIN_TYPE_SPACER),
//             NODE_PIN("Flip X", PIN_TYPE_BOOL),
//             NODE_PIN("Flip Y", PIN_TYPE_BOOL),
//             NODE_PIN("Rotation", PIN_TYPE_DOUBLE, 1.f),
//             NODE_PIN(PIN_TYPE_SPACER),
//             NODE_PIN("Scale", PIN_TYPE_DOUBLE + 1, .25f),
//             NODE_PIN("Scale X", PIN_TYPE_DOUBLE + 1, .25f),
//             NODE_PIN("Scale Y", PIN_TYPE_DOUBLE + 1, .25f),
//             NODE_PIN(PIN_TYPE_SPACER),
//             NODE_PIN("Warp X", PIN_TYPE_DOUBLE + 1, .25f),
//             NODE_PIN("Warp Y", PIN_TYPE_DOUBLE + 1, .25f)
//         },
//         /* outputs */ {
//             NODE_PIN("Object", PIN_TYPE_GAMEOBJECT)
//         }
//     ) {};

//     bool compute() {
//         auto obj = std::get<sp_GameObjectData>(m_inputs[0]->m_value);

//         auto pos = std::get<cocos2d::CCPoint>(m_inputs[2]->m_value);
//         auto flipX = std::get<bool>(m_inputs[4]->m_value);
//         auto flipY = std::get<bool>(m_inputs[5]->m_value);
//         auto rotation = std::get<double>(m_inputs[6]->m_value);
//         auto scale = std::get<double>(m_inputs[8]->m_value);
//         auto scaleX = std::get<double>(m_inputs[9]->m_value);
//         auto scaleY = std::get<double>(m_inputs[10]->m_value);
//         auto warpX = std::get<double>(m_inputs[12]->m_value);
//         auto warpY = std::get<double>(m_inputs[13]->m_value);

//         if (pos != ccp(0, 0)) {
//             obj->m_objectValues[2] = std::to_string(obj->propertyAs<double>(2) + pos.x);
//             obj->m_objectValues[3] = std::to_string(obj->propertyAs<double>(3) + pos.y);
//         }

//         if (flipX != false) obj->m_objectValues[4] = flipX;
//         if (flipY != false) obj->m_objectValues[5] = flipY;
//         if (rotation != 0) {
//             if (obj->m_objectValues.find(6) == obj->m_objectValues.end()) obj->m_objectValues[6] = 0.f;
//             obj->m_objectValues[6] = std::get<double>(obj->m_objectValues[6]) + rotation;
//         }

//         if (scale != 1) {
//             if (obj->m_objectValues.find(32) == obj->m_objectValues.end()) obj->m_objectValues[32] = 1.f;
//             obj->m_objectValues[32] = std::get<double>(obj->m_objectValues[32]) * scale;
//         }
//         if (scaleX != 1) {
//             if (obj->m_objectValues.find(128) == obj->m_objectValues.end()) obj->m_objectValues[128] = 1.f;
//             obj->m_objectValues[128] = std::get<double>(obj->m_objectValues[128]) * scaleX;
//         }
//         if (scaleY != 1) {
//             if (obj->m_objectValues.find(129) == obj->m_objectValues.end()) obj->m_objectValues[129] = 1.f;
//             obj->m_objectValues[129] = std::get<double>(obj->m_objectValues[129]) * scaleY;
//         }

//         if (warpX != 1) {
//             if (obj->m_objectValues.find(131) == obj->m_objectValues.end()) obj->m_objectValues[131] = 1.f;
//             obj->m_objectValues[131] = std::get<double>(obj->m_objectValues[131]) * warpX;
//         }
//         if (warpY != 1) {
//             if (obj->m_objectValues.find(132) == obj->m_objectValues.end()) obj->m_objectValues[132] = 1.f;
//             obj->m_objectValues[132] = std::get<double>(obj->m_objectValues[132]) * warpY;
//         }

//         geode::log::info("Inputted object {} scale: {}", m_computeIteration, std::get<double>(obj->m_objectValues[129]));

//         m_outputs[0]->m_value = obj;
//         return true;
//     }
// };
