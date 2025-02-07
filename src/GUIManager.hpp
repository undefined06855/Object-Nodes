#pragma once
#include "nodes/nodes.hpp"

enum class LinkPin { From, To };

class GuiManager {
public:
    GuiManager();
    static GuiManager& get();

    float m_guiScale;
    bool m_guiShowing;

    std::vector<sp_LinkData> m_links;
    std::vector<sp_GuiNode> m_nodes;

    void setup();
    void draw();
    void destroy();
    void compute();

    bool canPinsConnect(int from, int to);

    sp_PinData pinDataForPinID(int id);
    std::vector<sp_LinkData> linkDataForPinID(int id);
    // sp_GuiNode nodeForNodeID(unsigned int id);
    sp_GuiNode nodeForPinID(int id);
};
