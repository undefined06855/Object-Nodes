#include "CCEGLView.hpp"
#include <imgui.h>

void HookedCCEGLView::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
    CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
    if (button == 0) return; // imgui-cocos already handles left click
    ImGui::GetIO().AddMouseButtonEvent(button, action);
}

void HookedCCEGLView::onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    CCEGLView::onGLFWMouseScrollCallback(window, xoffset, yoffset);
    ImGui::GetIO().AddMouseWheelEvent(xoffset, yoffset);
}
