#pragma once
#include "../GuiNode.hpp"
#include "Geode/binding/EditorUI.hpp"
#include <imgui.h>

class SelectedObjectsNode : public GuiNode {
public:
    std::string getName() const { return "SelectedObjectsNode"; }

    SelectedObjectsNode() : GuiNode(
        /* title */ "Selected Objects",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {},
        /* outputs */ {
            NODE_PIN("Selected Objects", PIN_TYPE_GAMEOBJECT)
        }
    ) {
        m_width = 100.f;
        m_beginParsingFromThisNode = true;
    };

    int m_lastComputeNumObjectsOutputted = 0;
    geode::Ref<cocos2d::CCArray> m_selectedObjs = nullptr;

    void drawExtra() {
        auto objStr = fmt::format("{} Objects", m_lastComputeNumObjectsOutputted);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_width / 2.f) - (ImGui::CalcTextSize(objStr.c_str()).x / 2.f));
        ImGui::TextUnformatted(objStr.c_str());

        // draw update button
        std::string updateStr = "Update";
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_width / 2.f) - (ImGui::CalcTextSize(updateStr.c_str()).x / 2.f));
        if (ImGui::Button(updateStr.c_str())) onInputChangeAndUpdatePreview();
    }

    bool compute() {
        auto eui = EditorUI::get();

        if (m_computeIteration == 0) {
            if (m_selectedObjs) m_selectedObjs->release();
            m_selectedObjs = eui->getSelectedObjects();
            m_selectedObjs->retain();
        }

        if (m_selectedObjs->count() == 0) {
            // no objects selected!!
            auto notice = std::make_shared<GameObjectData>(914);
            notice->m_objectValues[25] = "10"; // z order
            notice->m_objectValues[24] = "9"; // z layer
            notice->m_objectValues[128] = "0.65"; // scale x
            notice->m_objectValues[129] = "0.65"; // scale y
            notice->m_objectValues[31] = "bm8gb2JqZWN0cyBzZWxlY3RlZCE="; // "no objects selected!" in base64
            m_outputs[0]->m_value = notice;
            return true;
        }

        // if this is last iteration, return true
        m_lastComputeNumObjectsOutputted = m_selectedObjs->count();
        if (m_computeIteration + 1 > m_selectedObjs->count()) return true;

        // else set output value to be the next gameobject in m_selectedObjs
        auto obj = static_cast<GameObject*>(m_selectedObjs->objectAtIndex(m_computeIteration));
        m_outputs[0]->m_value = std::make_shared<GameObjectData>(obj);

        return false;
    }
};
