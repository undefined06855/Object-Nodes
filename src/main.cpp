#include "GUIManager.hpp"
#include "EditorTabNode.hpp"
#include <alphalaneous.editortab_api/include/EditorTabs.hpp>
#include <imgui-cocos.hpp>

$on_mod(Loaded) {
    ImGuiCocos::get()
        .setup([]{ GuiManager::get().setup(); })
        .draw([]{ GuiManager::get().draw(); })
        .setVisible(true); // visibility is controlled in GuiManager::m_guiShowing

    EditorTabs::get()->registerTab(TabType::BUILD, "node-builder"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> cocos2d::CCNode* {
        auto textLabelOn = cocos2d::CCLabelBMFont::create("uwu", "bigFont.fnt");
        textLabelOn->setScale(0.4f);
        auto textLabelOff = cocos2d::CCLabelBMFont::create("owo", "bigFont.fnt");
        textLabelOff->setScale(0.4f);

        EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

        return EditorTabNode::create();
    });
}
