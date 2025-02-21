#include "GUIManager.hpp"
#include "EditorTabNode.hpp"
#include "NodeManager.hpp"
#include <alphalaneous.editortab_api/include/EditorTabs.hpp>
#include <imgui-cocos.hpp>

$on_mod(Loaded) {
    ImGuiCocos::get()
        .setup([]{ GuiManager::get().setup(); NodeManager::get().setup(); })
        .draw([]{ GuiManager::get().draw(); })
        .setVisible(true); // visibility is controlled in GuiManager::m_barShowing

    EditorTabs::get()->registerTab(TabType::BUILD, "node-builder"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> cocos2d::CCNode* {
        auto selected = cocos2d::CCSprite::create("icon-selected.png"_spr);
        selected->setScale(.255f);
        auto deselected = cocos2d::CCSprite::create("icon-deselected.png"_spr);
        deselected->setScale(.255f);

        EditorTabUtils::setTabIcons(
            toggler,
            selected,
            deselected
        );

        return EditorTabNode::create();
    });
}
