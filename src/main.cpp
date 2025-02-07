#include "GUIManager.hpp"
#include <imgui-cocos.hpp>
#include <Geode/modify/CCEGLView.hpp>

$on_mod(Loaded) {
    ImGuiCocos::get()
        .setup([]{ GuiManager::get().setup(); })
        .draw([]{ GuiManager::get().draw(); })
        .setVisible(true);
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

#include <Geode/ui/Popup.hpp>
class MyPopup : public geode::Popup<> {
protected:
    bool setup() {
        setTitle("node javascrtipt real");

        scheduleUpdate();

        return true;
    }

public:
    static MyPopup* create() {
        auto ret = new MyPopup();
        if (ret->initAnchored(485.f, 290.f)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    void update(float dt) {
        GuiManager::get().m_guiShowing = true;
        GuiManager::get().m_guiScale = m_mainLayer->getScale();
    }

    void onClose(cocos2d::CCObject* sender) {
        // yeah fuck you
        // Popup::onClose(sender);
        // GuiManager::get().m_guiShowing = false;
        GuiManager::get().compute();
    }
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
    void onPlay(cocos2d::CCObject* sender) {
        MyPopup::create()->show();
    }
};
