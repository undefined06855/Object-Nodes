#include "GUINode.hpp"
#include <imnodes_internal.h>
#include "utils.hpp"
#include "GUIManager.hpp"

unsigned int g_lastNodeID = 0;
GuiNode::GuiNode(std::string title, unsigned int color, std::vector<sp_PinData> inputs, std::vector<sp_PinData> outputs)
    : m_title(title)
    , m_color(color)
    , m_inputs(inputs)
    , m_outputs(outputs)
    , m_width(170.f)
    , m_id(g_lastNodeID++) {}

void GuiNode::draw() {
    ImNodes::PushStyleVar(ImNodesStyleVar_NodePadding, ImVec2(8.f, 5.f));
    ImNodes::PushStyleVar(ImNodesStyleVar_LinkThickness, 4.f);
    ImNodes::PushStyleVar(ImNodesStyleVar_LinkLineSegmentsPerLength, .05f);
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, m_color);
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, on::utils::brightenColor(m_color, 16));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, on::utils::brightenColor(m_color, 24));
    ImNodes::BeginNode(m_id);
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_width);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(m_title.c_str());
    ImNodes::EndNodeTitleBar();

    for (auto input : m_inputs) {
        auto col = on::utils::pinColorForType(input->m_value);
        ImNodes::PushColorStyle(ImNodesCol_Pin, col);
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, on::utils::brightenColor(col, 16));
        ImNodes::BeginInputAttribute(input->m_id, on::utils::pinShapeForType(input->m_value));
        ImGui::TextUnformatted(input->m_name.c_str());
        ImNodes::EndInputAttribute();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }

    for (auto output : m_outputs) {
        auto col = on::utils::pinColorForType(output->m_value);
        ImNodes::PushColorStyle(ImNodesCol_Pin, col);
        ImNodes::PushColorStyle(ImNodesCol_PinHovered, on::utils::brightenColor(col, 16));
        ImNodes::BeginOutputAttribute(output->m_id, on::utils::pinShapeForType(output->m_value));
        // right justify the text
        auto text = output->m_name.c_str();
        auto textWidth = ImGui::CalcTextSize(text).x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + m_width - textWidth);
        ImGui::TextUnformatted(text);
        ImNodes::EndOutputAttribute();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }

    ImGui::PopTextWrapPos();
    ImNodes::EndNode();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
    ImNodes::PopStyleVar();
    ImNodes::PopStyleVar();
    ImNodes::PopStyleVar();
}

void GuiNode::computeAndPropagate() {
    compute();

    if (m_outputs.size() == 0) return;

    auto gm = GuiManager::get();
    for (auto output : m_outputs) {
        auto links = gm.linkDataForPinID(output->m_id);
        for (auto link : links) {
            auto pin = gm.pinDataForPinID(link->m_to);
            pin->m_value = output->m_value;
            auto node = gm.nodeForPinID(link->m_to);
            node->computeAndPropagate();
        }
    }
}
