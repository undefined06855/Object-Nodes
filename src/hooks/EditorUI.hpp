#pragma once
#include <Geode/modify/EditorUI.hpp>

class $modify(HookedEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer);
    
    void toggleMode(cocos2d::CCObject* sender);
    void onPlaytest(cocos2d::CCObject* sender);
};
