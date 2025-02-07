#pragma once
#include <Geode/ui/Popup.hpp>

class NodeEditorPopup : public geode::Popup<> {
public:
    static NodeEditorPopup* create();
    bool setup();

    void onRun(cocos2d::CCObject* sender);

    void onSave(cocos2d::CCObject* sender);
    void save(std::filesystem::path file);
    void onLoad(cocos2d::CCObject* sender);
    void load(std::string file);
};
