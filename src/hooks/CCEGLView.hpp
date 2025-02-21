#pragma once
#include <Geode/modify/CCEGLView.hpp>

class $modify(HookedCCEGLView, cocos2d::CCEGLView) {
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void onGLFWKeyCallbacK(GLFWwindow* window, int key, int scancode, int action, int mods);

    void toggleFullScreen(bool fullscreen, bool borderless, bool fix);
};
