#include "CCEGLView.hpp"
#include <imgui-cocos.hpp>
#include "../GUIManager.hpp"

void HookedCCEGLView::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
    CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
    if (button == 0) return; // imgui-cocos already handles left click
    ImGui::GetIO().AddMouseButtonEvent(button, action);
}

void HookedCCEGLView::onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    CCEGLView::onGLFWMouseScrollCallback(window, xoffset, yoffset);
    ImGui::GetIO().AddMouseWheelEvent(xoffset, yoffset);
}

void HookedCCEGLView::toggleFullScreen(bool fullscreen, bool borderless, bool fix) {
    if (!ImGuiCocos::get().isInitialized()) {
        CCEGLView::toggleFullScreen(fullscreen, borderless, fix);
        return;
    }

    // this is when imgui cocos destroy gets called and i want to call my shit here as well
    GuiManager::get().destroy();
    CCEGLView::toggleFullScreen(fullscreen, borderless, fix);
}
