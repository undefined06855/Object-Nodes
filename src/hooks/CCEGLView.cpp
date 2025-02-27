#include "CCEGLView.hpp"
#include <imgui-cocos.hpp>
#include "../GuiManager.hpp"

void HookedCCEGLView::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
    // check if need to eat
    bool eat = false;
    if (ImGui::GetIO().WantCaptureMouse) {
        if (auto mod = geode::Loader::get()->getLoadedMod("ninkaz.middle-click-panning")) {
            auto setting = mod->getSettingValue<std::string>("drag-button");
            if (setting == "Right Click" && button == GLFW_MOUSE_BUTTON_RIGHT) eat = true;
            if (setting == "Middle Click" && button == GLFW_MOUSE_BUTTON_MIDDLE) eat = true;
        }
    }

    if (!eat) CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
    if (button == 0) return; // imgui-cocos already handles left click
    ImGui::GetIO().AddMouseButtonEvent(button, action);
}

void HookedCCEGLView::onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    CCEGLView::onGLFWMouseScrollCallback(window, xoffset, yoffset);
    ImGui::GetIO().AddMouseWheelEvent(xoffset, yoffset);
}

void HookedCCEGLView::onGLFWKeyCallbacK(GLFWwindow* window, int key, int scancode, int action, int mods) {
    CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
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
