#pragma once
#include "GuiNode.hpp"

class GuiManager {
public:
    GuiManager();
    static GuiManager& get();

    bool m_barShowing;
    float m_expansionPercent;

    bool m_addNodeMenuShowing;
    bool m_addNodeMenuJustAppeared;

    bool m_showDebug;

    std::map<std::string, std::function<sp_GuiNode()>> m_addNodeMenuButtons;
    std::vector<std::string> m_addNodeMenuSplits;

    void setup();
    void destroy();

    void update(float dt);
    void draw();
    void drawAddNodeMenu();

    void updatePreviewIfContainsSelectedObjectsNode();
    void updatePreview();

    void openAddNodeMenu();
};
