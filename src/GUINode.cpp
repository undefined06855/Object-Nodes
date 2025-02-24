#include "GuiNode.hpp"
#include "imnodes_internal.h"
#include "structs.hpp"
#include "utils.hpp"
#include "NodeManager.hpp"
#include "GuiManager.hpp"

CollectingGuiNode::CollectingGuiNode(std::string title, unsigned int color, std::vector<sp_PinData> inputs, std::vector<sp_PinData> outputs)
    : GuiNode(title, color, inputs, outputs)
    , m_objects({}) {}

GuiNode::GuiNode(std::string title, unsigned int color, std::vector<sp_PinData> inputs, std::vector<sp_PinData> outputs)
    : m_beginParsingFromThisNode(false)
    , m_endParsingAtThisNode(false)
    , m_title(title)
    , m_color(color)
    , m_inputs(inputs)
    , m_outputs(outputs)
    , m_width(140.f)
    , m_id(NodeManager::get().getNextNodeID())
    , m_computedInputCount(0)
    , m_computeIteration(0) {}

void GuiNode::onInputChangeAndUpdatePreview() {
    onInputChange();
    GuiManager::get().updatePreview();
}

void GuiNode::onInputChange() {}

void GuiNode::draw() {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, m_color);
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, on::utils::brightenColor(m_color, 16));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, on::utils::brightenColor(m_color, 24));
    ImNodes::BeginNode(m_id);

    ImNodes::BeginNodeTitleBar();
    std::string name;
    if (GuiManager::get().m_showDebug) {
        name = on::utils::cutOffImGuiString(fmt::format("{} ({})", m_title, m_id), m_width); 
    } else {
        name = on::utils::cutOffImGuiString(m_title, m_width);
    }
    ImGui::TextUnformatted(name.c_str());
    ImNodes::EndNodeTitleBar();

    ImGui::Dummy(ImVec2(m_width, 0.f));

    for (auto output : m_outputs) {
        drawPin(PinOrientation::Output, output);
    }

    for (auto input : m_inputs) {
        drawPin(PinOrientation::Input, input);
    }

    drawExtra();

    ImNodes::EndNode();
    ImNodes::PopColorStyle();   
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

void GuiNode::drawExtra() {}

void GuiNode::drawPin(PinOrientation orientation, sp_PinData pinData) {
    if (pinData->m_isSpacerPin) {
        ImGui::TextUnformatted("");
        return;
    }

    std::string name;
    if (GuiManager::get().m_showDebug) {
        name = on::utils::cutOffImGuiString(fmt::format("{} ({})", pinData->m_name, pinData->m_id), m_width);
    } else {
        name = on::utils::cutOffImGuiString(pinData->m_name, m_width);
    }
    
    if (pinData->m_value.index() == 5) {
        // dropdown
        auto value = std::get<std::vector<const char*>>(pinData->m_value);
        ImGui::TextUnformatted(name.c_str());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(m_width - ImGui::CalcTextSize(name.c_str()).x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
        bool ret = ImGui::Combo("", &pinData->m_dropDownSelectedValue, value.data(), value.size());
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
        if (ret) onInputChangeAndUpdatePreview();
        return;
    }

    auto col = on::utils::pinColorForType(pinData->m_value);
    auto shape = on::utils::pinShapeForType(pinData->m_value);
    
    ImNodes::PushColorStyle(ImNodesCol_Pin, col);
    ImNodes::PushColorStyle(ImNodesCol_PinHovered, on::utils::brightenColor(col, 16));

    auto textWidth = ImGui::CalcTextSize(name.c_str()).x;

    if (orientation == PinOrientation::Input) {
        // input
        ImNodes::BeginInputAttribute(pinData->m_id, shape);
    } else {
        // output
        ImNodes::BeginOutputAttribute(pinData->m_id, shape);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + m_width - textWidth);
    }

    float inputBoxWidth = m_width - textWidth;

    auto linkCount = NodeManager::get().linkDataForPinID(pinData->m_id).size();
    if (linkCount > 0 || !pinData->m_showInput || orientation == PinOrientation::Output) {
        // this pin is a connected input, an input with input disabled or an output, dont draw input boxes or anything
        ImGui::TextUnformatted(name.c_str());
    } else {
        // draw input boxes or whatever
        switch (pinData->m_value.index()) {
            case 0: { // bool
                auto ptr = std::get_if<bool>(&pinData->m_value);
                ImGui::SetNextItemWidth(inputBoxWidth);

                bool ret = ImGui::Checkbox(name.c_str(), ptr);
                if (ret) onInputChangeAndUpdatePreview();

                break;
            }
    
            case 1: { // double
                auto ptr = std::get_if<double>(&pinData->m_value);

                ImGui::SetNextItemWidth(inputBoxWidth);
                double zero = 0.f;
                bool ret = ImGui::DragScalar(
                    name.c_str(),
                    ImGuiDataType_Double,
                    ptr,
                    ImGui::GetIO().KeyShift ? pinData->m_increment / 10.f : pinData->m_increment,
                    &zero,
                    &zero,
                    "%.2f"
                );
                if (ret) onInputChangeAndUpdatePreview();

                break;
            }
    
            case 2: { // CCPoint
                auto ptr = std::get_if<cocos2d::CCPoint>(&pinData->m_value);

                float temp[2] = { ptr->x, ptr->y };
                ImGui::SetNextItemWidth(inputBoxWidth);
                bool ret = ImGui::DragFloat2(
                    name.c_str(),
                    temp,
                    ImGui::GetIO().KeyShift ? pinData->m_increment / 10.f : pinData->m_increment,
                    0.f,
                    0.f,
                    "%.2f"
                );
                ptr->x = temp[0];
                ptr->y = temp[1];
                if (ret) onInputChangeAndUpdatePreview();
                
                break;
            }
    
            case 3: { // ccColor4F
                auto ptr = std::get_if<cocos2d::ccColor4F>(&pinData->m_value);

                float temp[4] = { ptr->r, ptr->g, ptr->b, ptr->a };

                ImGui::SetNextItemWidth(inputBoxWidth);
                bool ret = ImGui::ColorEdit4(name.c_str(), temp, ImGuiColorEditFlags_NoInputs);
                
                ptr->r = temp[0];
                ptr->g = temp[1];
                ptr->b = temp[2];
                ptr->a = temp[3];

                if (ret) onInputChangeAndUpdatePreview();
            
                break;
            }
    
            case 4: { // sp_GameObjectData
                ImGui::TextUnformatted(name.c_str());

                break;
            }
        }
    }

    if (orientation == PinOrientation::Input) {
        ImNodes::EndInputAttribute();
    } else {
        ImNodes::EndOutputAttribute();
    }

    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

void GuiNode::computeAndPropagate() {
    auto& nm = NodeManager::get();

    geode::log::debug("{} #{} compute...", getName(), m_id);
    geode::log::NestScope scope;

    // see if all connected inputs have computed
    m_computedInputCount++;
    int connectedInputCount = 0;
    for (auto input : m_inputs) {
        if (on::utils::shouldSkipPin(input)) continue;
        auto linksConnected = nm.linkDataForPinID(input->m_id).size();
        connectedInputCount += linksConnected;
        // gameobject inputs need to be connected
        if (input->m_value.index() == 4 && linksConnected == 0) connectedInputCount += 1;
    }

    geode::log::debug("{} computed inputs / {} connected inputs -> {}", m_computedInputCount, connectedInputCount, m_computedInputCount >= connectedInputCount);

    if (m_computedInputCount <= connectedInputCount) return;

    geode::log::debug("All inputs computed, compute and propagate...");

    // keep computing and propagating to children until it returns true
    while (true) {
        bool isFinalCompute = compute();
        m_computeIteration++;

        geode::log::debug("Compute returned {}", isFinalCompute);
        geode::log::pushNest();
        for (auto output : m_outputs) {
            auto links = nm.linkDataForPinID(output->m_id);
            if (links.size() == 0) geode::log::debug("No links connected to output pin!");
            // two loops just in case!
            for (auto link : links) {
                auto pin = nm.pinDataForPinID(link->m_to);
                pin->m_value = output->m_value;
            }
            for (auto link : links) {
                auto node = nm.nodeForPinID(link->m_to);
                node->computeAndPropagate();
            }
        }
        geode::log::popNest();
        
        if (m_outputs.size() == 0) geode::log::debug("No outputs on this node!");
        if (isFinalCompute) break;
    }
}


// code taken from what BeginNode does
// hopefully calling ObjectPoolFindOrCreateIndex outside of the render loop
// doesn't fuck things up too much

cocos2d::CCPoint GuiNode::getNodePos() {
    ImNodesEditorContext& editor = ImNodes::EditorContextGet();
    const int node_idx = ImNodes::ObjectPoolFindOrCreateIndex(editor.Nodes, m_id);
    GImNodes->CurrentNodeIdx = node_idx;
    ImNodeData& node = editor.Nodes.Pool[node_idx];
    return { node.Origin.x, node.Origin.y };
}

void GuiNode::setNodePos(cocos2d::CCPoint pos) {
    ImNodesEditorContext& editor = ImNodes::EditorContextGet();
    const int node_idx = ImNodes::ObjectPoolFindOrCreateIndex(editor.Nodes, m_id);
    GImNodes->CurrentNodeIdx = node_idx;
    ImNodeData& node = editor.Nodes.Pool[node_idx];
    node.Origin = ImVec2(pos.x, pos.y);
}
