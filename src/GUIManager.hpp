#pragma once
#include "nodes/nodes.hpp"
#include <deque>

class GuiManager {
public:
    GuiManager();
    static GuiManager& get();

    bool m_barShowing;

    std::vector<sp_LinkData> m_links;
    std::vector<sp_GuiNode> m_nodes;

    std::vector<unsigned int> m_nodeIDsInUse;
    std::vector<int> m_pinIDsInUse;

    std::deque<unsigned int> m_recentlyDeletedNodeIDs;
    std::deque<int> m_recentlyDeletedPinIDs;

    void setup();
    void draw();
    void destroy();
    void compute();

    bool canPinsConnect(int from, int to);

    sp_PinData pinDataForPinID(int id);
    std::vector<sp_LinkData> linkDataForPinID(int id);
    // sp_GuiNode nodeForNodeID(unsigned int id);
    bool nodeIDExists(unsigned int id);
    bool pinIDExists(int id);
    sp_GuiNode nodeForPinID(int id);

    unsigned int getNextNodeID();
    int getNextPinID();

    void deleteNode(unsigned int id);
    void deleteLink(int id);
};
