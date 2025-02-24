#include "GuiManager.hpp"
#include "EditorTabNode.hpp"
#include "Geode/loader/Event.hpp"
#include "Geode/loader/EventV2.hpp"
#include "Geode/loader/ModEvent.hpp"
#include "NodeManager.hpp"
#include <alphalaneous.editortab_api/include/EditorTabs.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <imgui-cocos.hpp>

$on_mod(Loaded) {
    ImGuiCocos::get()
        .setup([]{ GuiManager::get().setup(); NodeManager::get().setup(); })
        .draw([]{ GuiManager::get().draw(); })
        .setVisible(true); // visibility is controlled in GuiManager::m_barShowing

    EditorTabs::get()->registerTab(TabType::BUILD, "node-builder"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> cocos2d::CCNode* {
        auto selected = cocos2d::CCSprite::create("icon-selected.png"_spr);
        selected->setScale(.255f);
        auto deselected = cocos2d::CCSprite::create("icon-deselected.png"_spr);
        deselected->setScale(.255f);

        EditorTabUtils::setTabIcons(
            toggler,
            selected,
            deselected
        );

        return EditorTabNode::create();
    });

    keybinds::BindManager::get()->registerBindable({
        /* id */ "open-add-node-menu"_spr,
        /* name */ "Open Add Node Menu",
        /* desc */ "A shortcut to open the Add Node menu, instead of using right click.",
        /* bind */ { keybinds::Keybind::create(cocos2d::KEY_A, keybinds::Modifier::Shift) },
        /* category */ "Editor/Object Nodes"
    });

    keybinds::BindManager::get()->registerBindable({
        /* id */ "expand-node-editor"_spr,
        /* name */ "Expand Node Editor",
        /* desc */ "A shortcut to open the Node Editor fully, when the tab is selected.",
        /* bind */ { keybinds::Keybind::create(cocos2d::KEY_E, keybinds::Modifier::Control) },
        /* category */ "Editor/Object Nodes"
    });
}

$on_mod(Loaded) {
    // you have to put a capture list in each function so that the compiler
    // knows to choose the std::function ctor of geode::EventListener

    new geode::EventListener([=](keybinds::InvokeBindEvent* event) {
        if (ImGui::GetIO().WantCaptureMouse) {
            GuiManager::get().openAddNodeMenu();
            return geode::ListenerResult::Stop;
        }
        
        return geode::ListenerResult::Propagate;
    }, keybinds::InvokeBindFilter(nullptr, "open-add-node-menu"_spr));

    new geode::EventListener([=](keybinds::InvokeBindEvent* event) {
        if (auto tab = EditorTabNode::get()) {
            tab->onToggleExpand(nullptr);
            return geode::ListenerResult::Stop;
        }

        return geode::ListenerResult::Propagate;
    }, keybinds::InvokeBindFilter(nullptr, "expand-node-editor"_spr));
}
