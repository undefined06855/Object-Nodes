#pragma once
#include <Geode/modify/CCEGLView.hpp>

class $modify(HookedCCEGLView, cocos2d::CCEGLView) {
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
