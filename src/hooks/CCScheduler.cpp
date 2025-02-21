#include "CCScheduler.hpp"
#include "../GUIManager.hpp"

void HookedCCScheduler::update(float dt) {
    CCScheduler::update(dt);

    GuiManager::get().update(dt);
}
