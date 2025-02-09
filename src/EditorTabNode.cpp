#include "EditorTabNode.hpp"
#include "GUIManager.hpp"
#include "utils.hpp"

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

    float targetBtnWidth = 34.f;

    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu");
    m_buttonMenu->ignoreAnchorPointForPosition(false);
    m_buttonMenu->setAnchorPoint({ 1.f, .5f });
    m_buttonMenu->setContentSize({ 40.f, getContentHeight() - 5.f });
    m_buttonMenu->setLayout(
        geode::ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setGap(.3f)
    );
    addChildAtPosition(m_buttonMenu, geode::Anchor::Right);

    auto applyBtnSprite = ButtonSprite::create("Apply");
    applyBtnSprite->setScale(targetBtnWidth / applyBtnSprite->getScaledContentWidth());
    auto applyBtn = CCMenuItemSpriteExtra::create(applyBtnSprite, this, menu_selector(EditorTabNode::onCommit));
    applyBtn->setID("apply-button");
    m_buttonMenu->addChild(applyBtn);

    m_buttonMenu->addChild(on::utils::createHR());

    auto saveBtnSprite = ButtonSprite::create("Save");
    saveBtnSprite->setScale(targetBtnWidth / saveBtnSprite->getScaledContentWidth());
    auto saveBtn = CCMenuItemSpriteExtra::create(saveBtnSprite, this, menu_selector(EditorTabNode::onSave));
    saveBtn->setID("save-button");
    m_buttonMenu->addChild(saveBtn);

    auto loadBtnSprite = ButtonSprite::create("Load");
    loadBtnSprite->setScale(targetBtnWidth / loadBtnSprite->getScaledContentWidth());
    auto loadBtn = CCMenuItemSpriteExtra::create(loadBtnSprite, this, menu_selector(EditorTabNode::onLoad));
    loadBtn->setID("load-button");
    m_buttonMenu->addChild(loadBtn);

    m_buttonMenu->updateLayout();

    return true;
}

void EditorTabNode::setVisible(bool visible) {
    CCNode::setVisible(visible);
    GuiManager::get().m_barShowing = visible;
}

void EditorTabNode::onCommit(cocos2d::CCObject* sender) {
    GuiManager::get().compute();
}

void EditorTabNode::onSave(cocos2d::CCObject* sender) {

}

void EditorTabNode::onLoad(cocos2d::CCObject* sender) {

}
