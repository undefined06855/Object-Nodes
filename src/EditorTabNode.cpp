#include "EditorTabNode.hpp"
#include "GUIManager.hpp"

EditorTabNode* EditorTabNode::create() {
    auto ret = new EditorTabNode;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool EditorTabNode::init() {
    if (!CCNode::init()) return false;
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    setAnchorPoint({ .5f, 0.f });
    setContentSize({ 370.f, 90.f });
    setPosition({ winSize.width / 2.f, 0.f });

    m_buttonMenu = cocos2d::CCMenu::create();
    m_mainNode = cocos2d::CCNode::create();
    addChild(m_buttonMenu);
    addChild(m_mainNode);

    m_buttonMenu->setContentSize(getContentSize());
    m_mainNode->setContentSize(getContentSize());

    m_mainNode->addChildAtPosition(cocos2d::CCLabelBMFont::create("skibidi", "bigFont.fnt"), geode::Anchor::Center);

    return true;
}

void EditorTabNode::setVisible(bool visible) {
    CCNode::setVisible(visible);
    GuiManager::get().m_barShowing = visible;
}
