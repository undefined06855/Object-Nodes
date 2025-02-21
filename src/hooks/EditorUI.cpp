#include "EditorUI.hpp"
#include "../EditorTabNode.hpp"

bool HookedEditorUI::init(LevelEditorLayer* levelEditorLayer) {
    if (!EditorUI::init(levelEditorLayer)) return false;

    // probably not great for mod compat but nobody else touches this anyway
    auto bg = getChildByID("background-sprite");
    bg->setPositionY(bg->getScaledContentHeight() - 1);
    bg->setAnchorPoint({ 0.f, 1.f });

    return true;
}

void HookedEditorUI::toggleMode(cocos2d::CCObject* sender) {
    auto tabNode = EditorTabNode::get();
    // dont let user change tabs if expanded
    if (tabNode && tabNode->m_expanded) return;

    EditorUI::toggleMode(sender);
}

void HookedEditorUI::onPlaytest(cocos2d::CCObject* sender) {
    // BUG: if user holds down button, presses enter, then releases, they can
    // make it expand while invisible
    // it does stop playtest because of fallback check so you dont get
    // softlocked but still not great
    
    auto tabNode = EditorTabNode::get();
    // dont let user playtest if expanded
    if (tabNode && tabNode->m_expanded) return;

    EditorUI::onPlaytest(sender);
}

