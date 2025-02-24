#include "CCScheduler.hpp"
#include "../GuiManager.hpp"

void HookedCCScheduler::update(float dt) {
    CCScheduler::update(dt);

    GuiManager::get().update(dt);
}
