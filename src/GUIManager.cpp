#include "GUIManager.hpp"
#include "utils.hpp"
#include <imnodes.h>

GuiManager::GuiManager()
    : m_guiScale(1.f)
    , m_guiShowing(false)
    , m_links({}) {}

GuiManager& GuiManager::get() {
    static GuiManager instance;
    return instance;
}

void GuiManager::setup() {
    ImNodes::CreateContext();

    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyShift;
    ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    ImNodes::GetIO().MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyShift;

    m_nodes.push_back(std::make_shared<TestConstantsNode>());
    m_nodes.push_back(std::make_shared<TestMultiply>());
    m_nodes.push_back(std::make_shared<TestOutput>());
}

void GuiManager::destroy() {
    ImNodes::DestroyContext();
}

void GuiManager::draw() {
    if (!m_guiShowing) return;
    
    auto director = cocos2d::CCDirector::sharedDirector();
    auto height = director->m_obResolutionInPixels.height;
    cocos2d::CCSize size = {
        height * .85f * 1.6f,
        height * .85f
    };
    size *= m_guiScale;
    cocos2d::CCPoint pos = {
        (director->m_obResolutionInPixels.width / 2.f) - (size.width / 2.f),
        (director->m_obResolutionInPixels.height / 2.f) - (size.height / 2.f),
    };
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(size.width, size.height));

    // draw main node editor
    // nodes and links
    ImGui::Begin("node editor", nullptr, ImGuiWindowFlags_NoDecoration);

    ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(125, 125, 125, 255));
    ImNodes::BeginNodeEditor();
    
    for (auto node : m_nodes) {
        node->draw();
    }

    int i = 0;
    for (auto link : m_links) {
        auto col = on::utils::pinColorForType(pinDataForPinID(link->m_from)->m_value);
        ImNodes::PushColorStyle(ImNodesCol_Link, col);
        ImNodes::PushColorStyle(ImNodesCol_LinkHovered, on::utils::brightenColor(col, 16));
        ImNodes::PushColorStyle(ImNodesCol_LinkSelected, on::utils::brightenColor(col, 24));
        ImNodes::Link(i++, link->m_from, link->m_to);
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);

    ImNodes::EndNodeEditor();
    ImNodes::PopColorStyle();

    ImGui::End();

    // check for link creation or deletion
    int from, to;
    if (ImNodes::IsLinkCreated(&from, &to)) {
        if (canPinsConnect(from, to)) {
            // find an already connected link and disconnect
            // should only be one but this can handle multiple
            auto links = linkDataForPinID(to);
            for (auto link : links) {
                m_links.erase(std::remove(m_links.begin(), m_links.end(), link), m_links.end());
            }

            m_links.push_back(std::make_shared<LinkData>(from, to));
        }
    }

    int id;
    if (ImNodes::IsLinkDestroyed(&id)) {
        m_links.erase(m_links.begin() + id);
    }

    // zoom only available because we're using Auburn's fork of imnodes
    // see Nelarius/imnodes #192
    if (ImNodes::IsEditorHovered() && ImGui::GetIO().MouseWheel != 0) {
        float zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
        ImNodes::EditorContextSetZoom(zoom, ImGui::GetMousePos());
    }
}

sp_PinData GuiManager::pinDataForPinID(int id) {
    for (auto node : m_nodes) {
        for (auto input : node->m_inputs) {
            if (input->m_id == id) return input;
        }

        for (auto output : node->m_outputs) {
            if (output->m_id == id) return output;
        }
    }

    geode::log::error("Could not find pin data for pin {}!", id);
    return nullptr;
}

bool GuiManager::canPinsConnect(int from, int to) {
    auto dataFrom = pinDataForPinID(from);
    auto dataTo = pinDataForPinID(to);

    return dataFrom->m_value.index() == dataTo->m_value.index();
}

void GuiManager::compute() {
    for (auto node : m_nodes) {
        if (node->m_inputs.size() == 0) {
            node->computeAndPropagate();
        }
    }
}

std::vector<sp_LinkData> GuiManager::linkDataForPinID(int id) {
    std::vector<sp_LinkData> ret = {};
    for (auto link : m_links) {
        if (link->m_from == id || link->m_to == id) ret.push_back(link);
    }

    return ret;
}

// sp_GuiNode GuiManager::nodeForNodeID(unsigned int id) {
//     for (auto node : m_nodes) {
//         if (node->m_id == id) return node;
//     }

//     geode::log::error("Could not find node data for node {}!", id);
//     return nullptr;
// }

sp_GuiNode GuiManager::nodeForPinID(int id) {
    for (auto node : m_nodes) {
        for (auto input : node->m_inputs) {
            if (input->m_id == id) return node;
        }

        for (auto output : node->m_outputs) {
            if (output->m_id == id) return node;
        }
    }

    geode::log::error("Could not find node data for pin {}!", id);
    return nullptr;
}
