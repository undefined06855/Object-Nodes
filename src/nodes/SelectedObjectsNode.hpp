#pragma once
#include "../GUINode.hpp"
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
        auto str = fmt::format("{} Objects", m_lastComputeNumObjectsOutputted);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_width / 2.f) - (ImGui::CalcTextSize(str.c_str()).x / 2.f));
        ImGui::TextUnformatted(str.c_str());
    }

    bool compute() {
        auto eui = EditorUI::get();
        if (!eui) return true; // idk

        if (m_computeIteration == 0) {
            m_selectedObjs = nullptr;
            auto objs = eui->getSelectedObjects();
            m_selectedObjs = objs;
        }

        m_lastComputeNumObjectsOutputted = m_selectedObjs->count();
        if (m_computeIteration + 1 > m_selectedObjs->count()) return true;

        // TODO: crash when zero objects selected because try to call
        // commitToString on nullptr
        // because m_outputs[0] is uninitialised and in fact never gets set
        // because m_selectedObjs->count() is zero

        auto obj = static_cast<GameObject*>(m_selectedObjs->objectAtIndex(m_computeIteration));
        m_outputs[0]->m_value = std::make_shared<GameObjectData>(obj);

        return false;
    }
};
