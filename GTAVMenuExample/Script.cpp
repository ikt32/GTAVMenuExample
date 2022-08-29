#include "Script.hpp"
#include "MenuTexture.hpp"
#include "ScriptMenu.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"
#include "Util/UI.hpp"

#include <inc/main.h>

namespace {
    std::shared_ptr<CMenuExampleScript> coreScript;
    std::unique_ptr<CScriptMenu<CMenuExampleScript>> scriptMenu;

    bool initialized = false;
}

// These functions are only called in Script.cpp so don't need to be exposed.
namespace MenuExample {
    void scriptInit();
    void scriptTick();
}

// ScriptHookV calls ScriptMain when loading a save,
// so it can happen multiple times in a game session.
void MenuExample::ScriptMain() {
    // This check exists to prevent global objects from being
    // initialized multiple times.
    if (!initialized) {
        LOG(INFO, "Script started");
        scriptInit();
        initialized = true;
    }
    else {
        LOG(INFO, "Script restarted");
    }

    scriptTick();
}

void MenuExample::scriptInit() {
    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";

    coreScript = std::make_shared<CMenuExampleScript>();

    // The menu being initialized. Note the passed settings,
    // the onInit and onExit lambdas and finally BuildMenu being called.
    scriptMenu = std::make_unique<CScriptMenu<CMenuExampleScript>>(settingsMenuPath.string(),
        []() {
            LOG(INFO, "Menu opened");
            // When the menu is opened, functions can be called.
            // In this case, the images in the example folder are refreshed.
            MenuTexture::UpdateTextures();
        },
        []() {
            LOG(INFO, "Menu closed");
            // When the menu is closed, functions can be called.
            // In this case, a notification is shown.
            UI::Notify("Menu was closed");
        },
        // The menu is built and submenus are passed into the CScriptMenu object.
        // See MenuExampleMenu.cpp for the actual implementation.
        BuildMenu()
    );
}

// This function starts the infinite loop that updates the script instance and menu, every game tick.
// It keeps running forever, until ScriptHookV restarts or stops the script.
void MenuExample::scriptTick() {
    while (true) {
        coreScript->Tick();
        scriptMenu->Tick(*coreScript);
        WAIT(0);
    }
}
