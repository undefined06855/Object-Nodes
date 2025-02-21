#include "CCKeyboardDispatcher.hpp"
#include "Geode/loader/ModEvent.hpp"
#include <imgui-cocos.hpp>

// taken from imgui-cocos
bool _shouldBlockInput() {
	auto& inst = ImGuiCocos::get();
	return inst.isVisible() && inst.getInputMode() == ImGuiCocos::InputMode::Blocking;
}

// taken from imgui-cocos
ImGuiKey _cocosToImGuiKey(cocos2d::enumKeyCodes key) {
	if (key >= cocos2d::KEY_A && key <= cocos2d::KEY_Z) {
		return static_cast<ImGuiKey>(ImGuiKey_A + (key - cocos2d::KEY_A));
	}
	if (key >= cocos2d::KEY_Zero && key <= cocos2d::KEY_Nine) {
		return static_cast<ImGuiKey>(ImGuiKey_0 + (key - cocos2d::KEY_Zero));
	}
	switch (key) {
		case cocos2d::KEY_Up: return ImGuiKey_UpArrow;
		case cocos2d::KEY_Down: return ImGuiKey_DownArrow;
		case cocos2d::KEY_Left: return ImGuiKey_LeftArrow;
		case cocos2d::KEY_Right: return ImGuiKey_RightArrow;

		case cocos2d::KEY_Control: return ImGuiKey_ModCtrl;
		case cocos2d::KEY_Shift: return ImGuiKey_ModShift;
		case cocos2d::KEY_Alt: return ImGuiKey_ModAlt;
		case cocos2d::KEY_Enter: return ImGuiKey_Enter;

		case cocos2d::KEY_Home: return ImGuiKey_Home;
		case cocos2d::KEY_End: return ImGuiKey_End;
		case cocos2d::KEY_Delete: return ImGuiKey_Delete;

        // added
        case cocos2d::KEY_Backspace: return ImGuiKey_Backspace;

		default: return ImGuiKey_None;
	}
}

// taken from imgui-cocos
bool HookedCCKeyboardDispatcher::dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat) {
    if (!ImGuiCocos::get().isInitialized())
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);

    const bool shouldEatInput = /* modified */ ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureMouse || _shouldBlockInput();
    // modified
    if (shouldEatInput) {
        const auto imKey = _cocosToImGuiKey(key);
        if (imKey != ImGuiKey_None) {
            ImGui::GetIO().AddKeyEvent(imKey, down);
        }
    }
    if (shouldEatInput) {
        return false;
    } else {
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
    }
}

void HookedCCKeyboardDispatcher::onModify(auto &self) {
    // to differentiate it from the imgui cocos hook to check later on
    (void)self.setHookPriority("cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG", 69);
}

// disable imgui-cocos's hook so only mine runs
$on_mod(Loaded) {
    auto hooks = geode::Mod::get()->getHooks();
    for (auto hook : hooks) {
        if (hook->getDisplayName() == "cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG" && hook->getPriority() != 69) {
            (void)hook->disable();
            break;
        }
    }
}
