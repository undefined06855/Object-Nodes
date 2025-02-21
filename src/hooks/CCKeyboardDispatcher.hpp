#pragma once
#include <Geode/modify/CCKeyboardDispatcher.hpp>

class $modify(HookedCCKeyboardDispatcher, cocos2d::CCKeyboardDispatcher) {
    static void onModify(auto& self);
	bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool down, bool repeat);
};
