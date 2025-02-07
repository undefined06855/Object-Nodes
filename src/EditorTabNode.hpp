#pragma once
#include <Geode/cocos/base_nodes/CCNode.h>

class EditorTabNode : public cocos2d::CCNode {
public:
    static EditorTabNode* create();
    bool init();

    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCNode* m_mainNode;

    void setVisible(bool visible);

    void onRun(cocos2d::CCObject* sender);
};
