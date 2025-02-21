#pragma once
#include <Geode/cocos/sprite_nodes/CCSprite.h>

class EditorTabNode : public cocos2d::CCNode {
public:
    static EditorTabNode* create();
    static EditorTabNode* get();
    bool init();

    bool m_expanded;
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCSprite* m_toggleExpandSprite;

    cocos2d::CCNodeRGBA* m_previewResultLayer;

    void setVisible(bool visible);

    void onCommit(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void onLoad(cocos2d::CCObject* sender);
    void onToggleExpand(cocos2d::CCObject* sender);
    void onToggleExpandUnanimatableProperties();

    void updatePreview(cocos2d::CCNode* node);
};
