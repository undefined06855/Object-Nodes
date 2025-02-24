#include "EditorTabNode.hpp"
#include "GuiManager.hpp"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "NodeManager.hpp"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/binding/EditorUI.hpp"
#include "Geode/cocos/actions/CCActionInterval.h"
#include "Geode/ui/Layout.hpp"
#include "Geode/utils/cocos.hpp"

EditorTabNode* EditorTabNode::create() {
    auto ret = new EditorTabNode;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

EditorTabNode* EditorTabNode::get() {
    if (!EditorUI::get()) return nullptr;
    return geode::cast::typeinfo_cast<EditorTabNode*>(EditorUI::get()->getChildByID("node-builder-bar"_spr));
}

bool EditorTabNode::init() {
    if (!CCNode::init()) return false;
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    m_expanded = false;

    setAnchorPoint({ .5f, 0.f });
    setContentSize({ 370.f, 90.f });
    setPosition({ winSize.width / 2.f, 0.f });

    m_buttonMenu = cocos2d::CCMenu::create();
    m_buttonMenu->setID("button-menu");
    m_buttonMenu->ignoreAnchorPointForPosition(false);
    m_buttonMenu->setAnchorPoint({ .5f, 1.f });
    m_buttonMenu->setContentSize({ 375.f, 14.f });
    if (geode::Loader::get()->isModLoaded("hjfod.betteredit")) {
        m_buttonMenu->setContentWidth(369.f);
        addChildAtPosition(m_buttonMenu, geode::Anchor::Top, { 0.f, -2.f });
    } else {
        addChildAtPosition(m_buttonMenu, geode::Anchor::Top, { -6.5f, -2.f });
    }

    auto applyBtnSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    applyBtnSprite->setScale(.175f);
    auto applyBtn = CCMenuItemSpriteExtra::create(applyBtnSprite, this, menu_selector(EditorTabNode::onCommit));
    applyBtn->setID("apply-button");
    m_buttonMenu->addChildAtPosition(applyBtn, geode::Anchor::Left, { 12.f, 0.f });

    auto saveBtnSprite = ButtonSprite::create("Save");
    saveBtnSprite->setScale(.375f);
    auto saveBtn = CCMenuItemSpriteExtra::create(saveBtnSprite, this, menu_selector(EditorTabNode::onSave));
    saveBtn->setID("save-button");
    m_buttonMenu->addChildAtPosition(saveBtn, geode::Anchor::Right, { -49.f, 0.f });

    auto loadBtnSprite = ButtonSprite::create("Load");
    loadBtnSprite->setScale(.375f);
    auto loadBtn = CCMenuItemSpriteExtra::create(loadBtnSprite, this, menu_selector(EditorTabNode::onLoad));
    loadBtn->setID("load-button");
    m_buttonMenu->addChildAtPosition(loadBtn, geode::Anchor::Right, { -15.f, 0.f });

    m_toggleExpandSprite = cocos2d::CCSprite::create("arrow-no-shadow.png"_spr);
    m_toggleExpandSprite->setScale(.5f);
    m_toggleExpandSprite->setFlipY(true);
    auto toggleExpandBtn = CCMenuItemSpriteExtra::create(m_toggleExpandSprite, this, menu_selector(EditorTabNode::onToggleExpand));
    toggleExpandBtn->setID("toggle-expand-button");
    m_buttonMenu->addChildAtPosition(toggleExpandBtn, geode::Anchor::Center);

    m_previewResultLayer = cocos2d::CCNodeRGBA::create();
    m_previewResultLayer->setID("preview-result-layer");
    m_previewResultLayer->setAnchorPoint({ .5f, 1.f });
    m_previewResultLayer->setContentSize({ getScaledContentWidth() * 1.5f, 100.f });
    m_previewResultLayer->setCascadeOpacityEnabled(true);
    m_previewResultLayer->setCascadeColorEnabled(true); // might as well
    addChildAtPosition(m_previewResultLayer, geode::Anchor::Top, { 0.f, -30.f });

    m_previewResultObjectCount = cocos2d::CCLabelBMFont::create("(0 objects)", "bigFont.fnt");
    m_previewResultObjectCount->setID("preview-result-count");
    m_previewResultObjectCount->setAnchorPoint({ 1.f, 0.f }); // bottom right
    m_previewResultObjectCount->setScale(.35f);
    m_previewResultLayer->addChildAtPosition(m_previewResultObjectCount, geode::Anchor::BottomRight, { -5.f, 5.f });

    return true;
}

void EditorTabNode::setVisible(bool visible) {
    CCNode::setVisible(visible);
    GuiManager::get().m_barShowing = visible;
}

void EditorTabNode::onCommit(cocos2d::CCObject* sender) {
    NodeManager::get().compute();
}

void EditorTabNode::onSave(cocos2d::CCObject* sender) {
    auto& nm = NodeManager::get();
    nm.m_nodeIDsInUse.clear();
    nm.m_pinIDsInUse.clear();
    nm.m_recentlyDeletedNodeIDs.clear();
    nm.m_recentlyDeletedNodeIDs.clear();
    nm.m_nodes.clear();
    nm.m_links.clear();
}

void EditorTabNode::onLoad(cocos2d::CCObject* sender) {
    NodeManager::get().setup();
}

#define EASE_ACTION(...) cocos2d::CCEaseInOut::create(__VA_ARGS__, 3.f)

void EditorTabNode::onToggleExpand(cocos2d::CCObject* sender) {
    if (m_toggleExpandSprite->numberOfRunningActions() > 0) {
        return;
    }

    m_toggleExpandSprite->runAction(EASE_ACTION(cocos2d::CCScaleBy::create(.3f, 1.f, -1.f)));

    m_expanded = !m_expanded;

    auto eui = EditorUI::get();

    // sorry 80 column enthusiasts
    if (m_expanded) {
        // just in case :)
        if (eui->m_playtestStopBtn->isVisible()) eui->onStopPlaytest(nullptr);

        eui->runAction(EASE_ACTION(cocos2d::CCMoveTo::create(.3f, { 0.f, eui->getContentHeight() - getContentHeight() + 5.f })));

        if (geode::Loader::get()->isModLoaded("hjfod.betteredit")) {
            eui->getChildByID("spacer-line-left")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -99.f, 0.f })));
            eui->getChildByID("toolbar-categories-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -99.f, 0.f })));
        } else {
            eui->getChildByID("spacer-line-left")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -87.f, 0.f })));
            eui->getChildByID("toolbar-categories-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -87.f, 0.f })));
        }

        eui->getChildByID("spacer-line-right")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 99.f, 0.f })));
        eui->getChildByID("toolbar-toggles-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 99.f, 0.f })));

        eui->getChildByID("background-sprite")->runAction(EASE_ACTION(cocos2d::CCScaleBy::create(.3f, 1.f, 3.55f)));

        m_buttonMenu->runAction(EASE_ACTION(cocos2d::CCScaleBy::create(.3f, 1.5f)));
        m_previewResultLayer->runAction(cocos2d::CCFadeIn::create(.3f));
    } else {
        eui->runAction(EASE_ACTION(cocos2d::CCMoveTo::create(.3f, { 0.f, 0.f })));
        
        if (geode::Loader::get()->isModLoaded("hjfod.betteredit")) {
            eui->getChildByID("spacer-line-left")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 99.f, 0.f })));
            eui->getChildByID("toolbar-categories-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 99.f, 0.f })));
        } else {
            eui->getChildByID("spacer-line-left")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 87.f, 0.f })));
            eui->getChildByID("toolbar-categories-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { 87.f, 0.f })));
        }

        eui->getChildByID("spacer-line-right")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -99.f, 0.f })));
        eui->getChildByID("toolbar-toggles-menu")->runAction(EASE_ACTION(cocos2d::CCMoveBy::create(.3f, { -99.f, 0.f })));

        eui->getChildByID("background-sprite")->runAction(EASE_ACTION(cocos2d::CCScaleBy::create(.3f, 1.f, 1.f / 3.5f)));

        m_buttonMenu->runAction(EASE_ACTION(cocos2d::CCScaleBy::create(.3f, 1.f / 1.5f)));
        m_previewResultLayer->runAction(cocos2d::CCFadeOut::create(.3f));
    }

    runAction(
        cocos2d::CCSequence::createWithTwoActions(
            cocos2d::CCDelayTime::create(.15f),
            geode::cocos::CallFuncExt::create([this] { onToggleExpandUnanimatableProperties(); })
        )
    );
}

// this gets called halfway through the animation by the sequence above so that
// properties that cant be animated (or i cant be bothered to) aren't jarring
// when they immediately snap to the new value
void EditorTabNode::onToggleExpandUnanimatableProperties() {
    auto eui = EditorUI::get();

    if (m_expanded) {
        m_buttonMenu->setLayoutOptions(
            geode::AnchorLayoutOptions::create()
                ->setOffset({ 0.f, -8.f })
                ->setAnchor(geode::Anchor::Top)
        );
    } else {
        geode::AnchorLayoutOptions* options;
        if (geode::Loader::get()->isModLoaded("hjfod.betteredit")) {
            options = geode::AnchorLayoutOptions::create()
                ->setOffset({ 0.f, -2.f })
                ->setAnchor(geode::Anchor::Top);
        } else {
            options = geode::AnchorLayoutOptions::create()
                ->setOffset({ -6.5f, -2.f })
                ->setAnchor(geode::Anchor::Top);
        }

        m_buttonMenu->setLayoutOptions(options);
    }
}

// technically only has to be ccnodergba and not ccsprite but we only ever use
// this with ccsprite anyway and just in case for the future
void EditorTabNode::updatePreview(cocos2d::CCSprite* node) {
    auto old = m_previewResultLayer->getChildByType<cocos2d::CCSprite>(0);
    if (old) old->removeFromParent();

    // fit node in space
    float previewRatio = m_previewResultLayer->getContentWidth() / m_previewResultLayer->getContentHeight();
    float nodeRatio = node->getContentWidth() / node->getContentHeight();

    float minSize = 30.f;
    if (previewRatio > nodeRatio) {
        // adjust for height
        float maxSize = m_previewResultLayer->getContentHeight() * 0.9f;
        if (node->getContentHeight() > maxSize) {
            node->setScale(maxSize / node->getContentHeight());
        } else if (node->getContentHeight() < minSize) {
            node->setScale(minSize / node->getContentHeight());
        }
    } else {
        // adjust for width
        float maxSize = m_previewResultLayer->getContentWidth() * 0.9f;
        if (node->getContentWidth() > maxSize) {
            node->setScale(maxSize / node->getContentWidth());
        } else if (node->getContentWidth() < minSize) {
            node->setScale(minSize / node->getContentWidth());
        }
    }

    m_previewResultLayer->addChildAtPosition(node, geode::Anchor::Center);
    // allow fading (and colour because why not)
    node->setCascadeOpacityEnabled(true);
    node->setCascadeColorEnabled(true);

    m_previewResultObjectCount->setString(fmt::format("({} objects)", node->getChildrenCount()).c_str());    
}
