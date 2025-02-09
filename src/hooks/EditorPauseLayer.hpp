#pragma once
#include <Geode/modify/EditorPauseLayer.hpp>

class $modify(HookedEditorPauseLayer, EditorPauseLayer) {
#ifndef GEODE_IS_MACOS
    void onExitEditor(cocos2d::CCObject* sender);
#else
    void FLAlert_Clicked(FLAlertLayer* p0, bool btnTwo);
#endif
    void onSaveAndExit(cocos2d::CCObject* sender);
    void onSaveAndPlay(cocos2d::CCObject* sender);
};
