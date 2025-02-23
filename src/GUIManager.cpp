#include "GUIManager.hpp"
#include "EditorTabNode.hpp"
#include "GUINode.hpp"
#include "nodes/MergeSplitColorNodes.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/nodes.hpp"
#include "utils.hpp"
#include "NodeManager.hpp"
#include <imgui.h>
#include <imnodes.h>

GuiManager::GuiManager()
    : m_barShowing(false)
    , m_expansionPercent(0.f)
    , m_addNodeMenuShowing(false)
    , m_showDebug(false)
    , m_addNodeMenuButtons({})
    , m_addNodeMenuSplits({}) {}

GuiManager& GuiManager::get() {
    static GuiManager instance;
    return instance;
}

#define NODE_MENU_BUTTON(Type) []{ return std::make_shared<Type>(); }

void GuiManager::setup() {
    ImNodes::CreateContext();

    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyShift;
    ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    ImNodes::GetIO().MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyShift;

    ImGuiIO& io = ImGui::GetIO();
    auto path = (geode::Mod::get()->getResourcesDir() / "NunitoSansMedium.ttf");
    io.FontDefault = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 16.f);

    m_addNodeMenuButtons = {
        { "Create Object", NODE_MENU_BUTTON(CreateObjectNode) },
        { "Selected Objects", NODE_MENU_BUTTON(SelectedObjectsNode) },
        { "Output", NODE_MENU_BUTTON(OutputNode) },
        // ------------------------------------------------
        { "Boolean", NODE_MENU_BUTTON(BooleanNode) },
        { "Value", NODE_MENU_BUTTON(ValueNode) },
        { "Position", NODE_MENU_BUTTON(PositionNode) },
        { "Color", NODE_MENU_BUTTON(ColorNode) },
        // ------------------------------------------------
        { "Merge Color", NODE_MENU_BUTTON(MergeColorNode) },
        { "Split Color", NODE_MENU_BUTTON(SplitColorNode) },
        // ------------------------------------------------
        { "Array", NODE_MENU_BUTTON(ArrayNode) }
    };

    // marks where horizontal lines are placed
    m_addNodeMenuSplits = {
        "Output",
        "Color",
        "Split Color"
    };
}

void GuiManager::destroy() {
    ImNodes::DestroyContext();
}

void GuiManager::update(float dt) {
    auto tab = EditorTabNode::get();
    if (!tab) return;

    if (tab->m_expanded && m_expansionPercent < 1.f) {
        m_expansionPercent += dt / .3f;
    } else if (!tab->m_expanded && m_expansionPercent > 0.f) {
        m_expansionPercent -= dt / .3f;
    }

    m_expansionPercent = std::min(std::max(m_expansionPercent, 0.f), 1.f);
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
    
    // easeinout implementation taken from cocos
    // https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-2.2.3/cocos2dx/actions/CCActionEase.cpp#L344-L355
    float easedExpansionPercent = m_expansionPercent;
    float rate = 3.f;
    easedExpansionPercent *= 2;
    if (easedExpansionPercent < 1) easedExpansionPercent = .5f * powf(easedExpansionPercent, rate);
    else                           easedExpansionPercent = 1.f - .5f * powf(2 - easedExpansionPercent, rate);

    // figure out scale between imgui to cocos dimensions
    // imgui-cocos provides utils but they work a bit differently and only work
    // between ccpoint <-> imvec2 which isnt really what i need for most of this
    auto director = cocos2d::CCDirector::sharedDirector();
    auto width = director->m_obResolutionInPixels.width;
    auto ccwidth = cocos2d::CCScene::get()->getScaledContentWidth();
    auto scaleFactor = width / ccwidth;

    // lerp between bar size (unexpanded) and fullscreen size (expanded) over easedExpansionPercent
    cocos2d::CCSize winSize = {
        std::lerp(380.f, cocos2d::CCScene::get()->getContentWidth() - 5.f, easedExpansionPercent) * scaleFactor,
        std::lerp(70.f, cocos2d::CCScene::get()->getContentHeight() - 135.f, easedExpansionPercent) * scaleFactor
    };

    // then centre it
    cocos2d::CCPoint pos = {
        (director->m_obResolutionInPixels.width / 2.f) - (winSize.width / 2.f),
        (director->m_obResolutionInPixels.height - winSize.height - (2.5f * scaleFactor))
    };

    // slide to the left a tad for bar size
    pos.x -= (1.f - easedExpansionPercent) * 6.5f * scaleFactor;

    // betteredit makes buttons on the left a bit wider
    if (geode::Loader::get()->isModLoaded("hjfod.betteredit")) {
        // shuffle left edge right a bit
        pos.x += (1.f - easedExpansionPercent) * 12.f * scaleFactor;
        winSize.width -= (1.f - easedExpansionPercent) * 12.f * scaleFactor;
    }

    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(winSize.width, winSize.height));


    // ImGuiWindowFlags_NoBringToFrontOnFocus used to prevent the node picker
    // window going behind the node editor window
    ImGui::Begin("node-editor"_spr, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(125, 125, 125, 255));
    auto btnCol = IM_COL32(43, 43, 43, 255);
    ImGui::PushStyleColor(ImGuiCol_Button, btnCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, on::utils::brightenColor(btnCol, 16));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, on::utils::brightenColor(btnCol, 24));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, btnCol);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, on::utils::brightenColor(btnCol, 16));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, on::utils::brightenColor(btnCol, 24));
    ImNodes::PushStyleVar(ImNodesStyleVar_NodePadding, ImVec2(8.f, 4.f));
    ImNodes::PushStyleVar(ImNodesStyleVar_LinkThickness, 3.f);
    ImNodes::PushStyleVar(ImNodesStyleVar_LinkLineSegmentsPerLength, .06f);
    ImNodes::BeginNodeEditor();
    
    auto& nm = NodeManager::get();

    for (auto node : nm.m_nodes) {
        node->draw();
    }

    int i = 0;

    for (auto link : nm.m_links) {
        auto col = on::utils::pinColorForType(nm.pinDataForPinID(link->m_from)->m_value);
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
    ImNodes::PopStyleVar(3);
    ImGui::PopStyleColor(6);
    ImNodes::PopColorStyle();

    ImGui::End();

    if (m_addNodeMenuShowing) drawAddNodeMenu();

    // check for link creation or deletion
    int from, to;
    if (ImNodes::IsLinkCreated(&from, &to)) {
        nm.onCreateLink(from, to);
    }

    int id;
    if (ImNodes::IsLinkDestroyed(&id)) {
        nm.deleteLink(id);
    }

    bool isDeleting = ImGui::IsKeyPressed(ImGuiKey_Delete, false) || (ImGui::IsKeyPressed(ImGuiKey_Backspace, false) && ImGui::GetIO().KeyAlt);
    // geode::log::info("{} || {} && {}", ImGui::IsKeyPressed(ImGuiKey_Delete, false), ImGui::IsKeyPressed(ImGuiKey_Backspace, false), ImGui::GetIO().KeyAlt);
    if (isDeleting) {
        if (ImNodes::IsLinkHovered(&id)) {
            nm.deleteLink(id);
        }
    
        if (ImNodes::IsNodeHovered(&id)) {
            nm.deleteNode(id);
        }

        std::vector<int> nodesToDelete = {};
        for (auto node : nm.m_nodes) {
            if (ImNodes::IsNodeSelected(node->m_id)) {
                nodesToDelete.push_back(node->m_id);
            }
        }

        for (auto id : nodesToDelete) {
            nm.deleteNode(id);
        }
    }

    // zoom only available because we're using Auburn's fork of imnodes
    // see Nelarius/imnodes #192
    if (ImNodes::IsEditorHovered() && ImGui::GetIO().MouseWheel != 0) {
        float zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
        ImNodes::EditorContextSetZoom(zoom, ImGui::GetMousePos());
    }
}

void GuiManager::drawAddNodeMenu() {
    ImGui::Begin("node-editor-add-node-menu"_spr, nullptr, ImGuiWindowFlags_NoDecoration);
    
    for (auto& pair : m_addNodeMenuButtons) {
        auto& label = pair.first;
        auto& callback = pair.second;

        bool ret = ImGui::Button(label.c_str());
        if (ret) {
            auto node = callback();
            NodeManager::get().m_nodes.push_back(node);
        }

        if (std::find(m_addNodeMenuSplits.begin(), m_addNodeMenuSplits.end(), label) != m_addNodeMenuSplits.end()) {
            // add break here
            ImGui::TextUnformatted("-------------");
        }
    }

    ImGui::End();
}

void GuiManager::updatePreview() {
    auto thread = std::thread([]{
        geode::utils::thread::setName("Object Nodes Compute Thread");

        NodeManager::get().compute();

        geode::Loader::get()->queueInMainThread([]{ 
            auto res = NodeManager::get().m_result;
            if (res.empty()) return;
        
            auto editor = LevelEditorLayer::get();
            if (!editor) return;
    
            // see https://github.com/FireMario211/Object-Workshop/blob/main/src/nodes/ObjectItem.cpp
            auto smartBlock = cocos2d::CCArray::create();
            int renderLimit = 4096;
            auto sprite = editor->m_editorUI->spriteFromObjectString(res, false, false, renderLimit, smartBlock, nullptr, nullptr);
            editor->updateObjectColors(smartBlock);
        
            EditorTabNode::get()->updatePreview(sprite);
        });
    });

    thread.detach();
}

// was going to be used for auto-update for selected objects
// maybe add if GameObject::deselectObject isnt INLINED ON WINDOWS
void GuiManager::updatePreviewIfContainsSelectedObjectsNode() {
    for (auto node : NodeManager::get().m_nodes) {
        if (node->getName() == "SelectedObjectsNode" && /* auto-update */ std::get<bool>(node->m_inputs[0]->m_value)) {
            updatePreview();
            return;
        }
    }
}
