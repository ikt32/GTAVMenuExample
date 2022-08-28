#pragma once
#include "MenuExampleScript.hpp"
#include "ScriptMenu.hpp"

/*
 * Script.hpp exposes functions to initialize and manage the script.
 */
namespace MenuExample {
    // ScriptHookV calls this when starting the script.
    void ScriptMain();

    /*
     * BuildMenu builds the menu with its options and submenus.
     * This function is declared here, so Script.cpp can call it
     * The implementation is in MenuExampleMenu.cpp
     */
    std::vector<CScriptMenu<CMenuExampleScript>::CSubmenu> BuildMenu();
}
