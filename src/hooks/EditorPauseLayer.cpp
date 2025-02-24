#include "EditorPauseLayer.hpp"
#include "../GuiManager.hpp"

// taken from editormusic
// hide bar whenever you exit the editor

#ifndef GEODE_IS_MACOS
void HookedEditorPauseLayer::onExitEditor(cocos2d::CCObject* sender) {
    GuiManager::get().m_barShowing = false;
    EditorPauseLayer::onExitEditor(sender);
}
#else
void HookedEditorPauseLayer::FLAlert_Clicked(FLAlertLayer* p0, bool btnTwo) {
    if (p0->getTag() == 1 && btnTwo) {
        GuiManager::get().m_barShowing = false;
    }
    EditorPauseLayer::FLAlert_Clicked(p0, btnTwo);
}
#endif

void HookedEditorPauseLayer::onSaveAndExit(cocos2d::CCObject* sender) {
    GuiManager::get().m_barShowing = false;
    EditorPauseLayer::onSaveAndExit(sender);
}

void HookedEditorPauseLayer::onSaveAndPlay(cocos2d::CCObject* sender) {
    GuiManager::get().m_barShowing = false;
    EditorPauseLayer::onSaveAndPlay(sender);
}
