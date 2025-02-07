#include "GUIManager.hpp"
#include <imgui-cocos.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <alphalaneous.editortab_api/include/EditorTabs.hpp>
#include "EditorTabNode.hpp"

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

class $modify(cocos2d::CCEGLView) {
	void toggleFullScreen(bool fullscreen, bool borderless, bool fix) {
		if (!ImGuiCocos::get().isInitialized()) {
            CCEGLView::toggleFullScreen(fullscreen, borderless, fix);
            return;
        }

        // this is when imgui cocos destroy gets called and i want to call my shit here as well
		GuiManager::get().destroy();
		CCEGLView::toggleFullScreen(fullscreen, borderless, fix);
	}
};
