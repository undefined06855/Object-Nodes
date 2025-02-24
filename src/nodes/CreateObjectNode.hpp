#pragma once
#include "../GuiNode.hpp"
#include "../NodeManager.hpp"
#include <imgui.h>

class CreateObjectNode : public GuiNode {
public:
    std::string getName() const { return "CreateObjectNode"; }

    CreateObjectNode() : GuiNode(
        /* title */ "Create Object",
        /* col */ NODE_COLOR_INPUT,
        /* inputs */ {
            NODE_PIN("Object ID", PIN_TYPE_DOUBLE, 0.f, false)
        },
        /* outputs */ {
            NODE_PIN("Object", PIN_TYPE_GAMEOBJECT)
        }
    ) {
        m_width = 100.f;
        m_beginParsingFromThisNode = true;
    };

    int m_objectID = 1;
    cocos2d::CCSpriteFrame* m_cachedObjectFrame = nullptr;
    int m_cachedObjectID = -1;

    void onInputChange() {
        m_inputs[0]->m_value = std::floor(std::get<double>(m_inputs[0]->m_value));
    }

    void drawExtra() {
        // see if input is connected and if so we dont need all this malarkey
        if (NodeManager::get().linkDataForPinID(m_inputs[0]->m_id).size() > 0) {
            return;
        }

        // get new frame if object id changes
        // else just keep previous one no need to get a new one
        if (m_cachedObjectID != m_objectID) {
            m_cachedObjectFrame = cocos2d::CCSpriteFrameCache::get()->spriteFrameByName(ObjectToolbox::sharedState()->intKeyToFrame(m_objectID));
            m_cachedObjectID = m_objectID;
        }

        // check if it was invalid
        if (!m_cachedObjectFrame || m_cachedObjectFrame == cocos2d::CCSpriteFrameCache::get()->spriteFrameByName("geode.texture-loader/fallback.png")) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            bool ret = ImGui::Button("Invalid Object!");
            if (ret) openPopup();
            ImGui::PopStyleColor();

            auto string = fmt::format("Object {}", m_objectID);
            auto width = ImGui::CalcTextSize(string.c_str()).x;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (m_width / 2.f) - (width / 2.f));
            ImGui::TextUnformatted(string.c_str());
            return;
        }

        // get corners and map to 0 - 1 for imgui
        auto uvRect = m_cachedObjectFrame->getRectInPixels();
        if (m_cachedObjectFrame->isRotated()) {
            auto width = uvRect.size.width;
            uvRect.size.width = uvRect.size.height;
            uvRect.size.height = width;
        }
        auto texSize = m_cachedObjectFrame->getTexture()->getContentSizeInPixels();
        uvRect.origin.x /= texSize.width;
        uvRect.origin.y /= texSize.height;
        uvRect.size.width /= texSize.width;
        uvRect.size.height /= texSize.height;

        // figure out size of the button
        // this was readable at one point but just devolved into this shit
        auto rect = m_cachedObjectFrame->getRectInPixels();
        if (m_cachedObjectFrame->isRotated()) {
            auto width = rect.size.width;
            rect.size.width = rect.size.height;
            rect.size.height = width;
        }
        float maxSize = 80.f;
        ImVec2 buttonSize = { maxSize, maxSize };
        if (rect.size.width < rect.size.height) {
            buttonSize.y *= (rect.size.height / rect.size.width);
            if (buttonSize.y > maxSize) {
                buttonSize.x *= maxSize / buttonSize.y;
                buttonSize.y = maxSize;
            }
        } else {
            buttonSize.x *= (rect.size.width / rect.size.height);
            if (buttonSize.x > maxSize) {
                buttonSize.y *= maxSize / buttonSize.x;
                buttonSize.x = maxSize;
            }
        }

        auto id = m_cachedObjectFrame->getTexture()->getName();

        // centre button horizontally
        auto origX = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(origX + (m_width / 2.f) - (buttonSize.x / 2.f));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        bool ret = ImGui::ImageButton((void*)id, buttonSize, ImVec2(uvRect.getMinX(), uvRect.getMinY()), ImVec2(uvRect.getMaxX(), uvRect.getMaxY()));
#pragma clang diagnostic pop
        if (ret) openPopup();

        auto string = fmt::format("Object {}", m_objectID);
        auto width = ImGui::CalcTextSize(string.c_str()).x;
        ImGui::SetCursorPosX(origX + (m_width / 2.f) - (width / 2.f));
        ImGui::TextUnformatted(string.c_str());
    }

    void openPopup() {
        m_objectID++;
        onInputChangeAndUpdatePreview();
    }

    bool compute() {
        // see if input is connected and if so use the input value
        // else use whatever's selected
        if (NodeManager::get().linkDataForPinID(m_inputs[0]->m_id).size() > 0) {
            m_outputs[0]->m_value = std::make_shared<GameObjectData>(std::get<double>(m_inputs[0]->m_value));
        } else {
            m_outputs[0]->m_value = std::make_shared<GameObjectData>(m_objectID);
        }
        return true;
    }
};
