#include "GUIManager.hpp"
#include "utils.hpp"
#include <Geode/binding/LevelEditorLayer.hpp>
#include <imnodes.h>

GuiManager::GuiManager()
    : m_barShowing(false)
    , m_links({})

    , m_nodeIDsInUse({})
    , m_pinIDsInUse({})
    
    , m_recentlyDeletedNodeIDs({})
    , m_recentlyDeletedPinIDs({}) {}

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
    if (!m_barShowing) return;

    int persistedNodes = geode::SceneManager::get()->getPersistedNodes().size();
    // taken from editormusic to see if there's any overlays
    // TODO: this may still run for geode notifications idk are they persisted
    int lowPassStrength = cocos2d::CCScene::get()->getChildrenCount() - persistedNodes - 1;
    if (LevelEditorLayer::get() && LevelEditorLayer::get()->getChildByID("EditorPauseLayer")) lowPassStrength++;
    if (cocos2d::CCScene::get()->getChildByID("thesillydoggo.qolmod/QOLModButton")) lowPassStrength--;
    if (cocos2d::CCScene::get()->getChildByID("hjfod.quick-volume-controls/overlay")) lowPassStrength--;

    if (lowPassStrength > 0) return;
    
    auto director = cocos2d::CCDirector::sharedDirector();
    auto width = director->m_obResolutionInPixels.width;
    auto ccwidth = cocos2d::CCScene::get()->getScaledContentWidth();
    auto scaleFactor = width / ccwidth;
    cocos2d::CCSize size = {
        370.f * scaleFactor,
        85.f * scaleFactor
    };
    cocos2d::CCPoint pos = {
        (director->m_obResolutionInPixels.width / 2.f) - (size.width / 2.f),
        (director->m_obResolutionInPixels.height - size.height - (2.5f * scaleFactor))
    };
    size.width -= 40.f * scaleFactor;
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
        deleteLink(id);
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

bool GuiManager::nodeIDExists(unsigned int id) {
    return std::find(m_nodeIDsInUse.begin(), m_nodeIDsInUse.end(), id) != m_nodeIDsInUse.end();
}

bool GuiManager::pinIDExists(int id) {
    return std::find(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), id) != m_pinIDsInUse.end();
}

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


unsigned int GuiManager::getNextNodeID() {
    if (!m_recentlyDeletedNodeIDs.empty()) {
        auto ret = m_recentlyDeletedNodeIDs.front();
        m_recentlyDeletedNodeIDs.pop_front();
        return ret;
    }

    // scary!
    unsigned int ret = 0;
    while (true) {
        if (!nodeIDExists(ret)) {
            m_nodeIDsInUse.push_back(ret);
            return ret;
        }
        ret++;
    }
}

int GuiManager::getNextPinID() {
    if (!m_recentlyDeletedPinIDs.empty()) {
        auto ret = m_recentlyDeletedPinIDs.front();
        m_recentlyDeletedPinIDs.pop_front();
        return ret;
    }

    // scary!
    int ret = 0;
    while (true) {
        if (!pinIDExists(ret)) {
            m_pinIDsInUse.push_back(ret);
            return ret;
        }
        ret++;
    }
}

void GuiManager::deleteNode(unsigned int id) {
    // >:(
    sp_GuiNode node;
    for (auto checkNode : m_nodes) {
        if (checkNode->m_id == id) {
            // ah ha
            node = checkNode;
            break;
        }
    }

    if (!node) {
        geode::log::error("Attempted to delete nonexistent node {}!", id);
        return;
    }

    m_recentlyDeletedNodeIDs.push_back(id);
    m_nodeIDsInUse.erase(std::remove(m_nodeIDsInUse.begin(), m_nodeIDsInUse.end(), id), m_nodeIDsInUse.end());

    for (auto pin : node->m_inputs) {
        m_recentlyDeletedPinIDs.push_back(pin->m_id);
        m_pinIDsInUse.erase(std::remove(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), pin->m_id), m_pinIDsInUse.end());
    }

    for (auto pin : node->m_outputs) {
        m_recentlyDeletedPinIDs.push_back(pin->m_id);
        m_pinIDsInUse.erase(std::remove(m_pinIDsInUse.begin(), m_pinIDsInUse.end(), pin->m_id), m_pinIDsInUse.end());
    }

    m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
}

void GuiManager::deleteLink(int id) {
    // link ids are always just the index they are in the list of links
    // nice and simple
    m_links.erase(m_links.begin() + id);
}
