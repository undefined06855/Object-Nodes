#pragma once
#include <Geode/cocos/base_nodes/CCNode.h>

class EditorTabNode : public cocos2d::CCNode {
public:
    static EditorTabNode* create();
    bool init();

    cocos2d::CCMenu* m_buttonMenu;

    void setVisible(bool visible);

    void onCommit(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void onLoad(cocos2d::CCObject* sender);
};
