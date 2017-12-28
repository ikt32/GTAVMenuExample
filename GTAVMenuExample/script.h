#pragma once

#include <string>

#include "..\..\ScriptHookV_SDK\inc\natives.h"
#include "..\..\ScriptHookV_SDK\inc\types.h"
#include "..\..\ScriptHookV_SDK\inc\enums.h"
#include "..\..\ScriptHookV_SDK\inc\main.h"

#include "menu.h"

#define DISPLAY_VERSION "v1.0.0"

const std::string modDir  = "\\GTAVMenuExample";

void ScriptMain();

// We're using the global variable "menu" in both script.cpp and ScriptMenu.cpp
extern NativeMenu::Menu menu;
extern int textureBgId;
extern int textureBgId2;

/*
 * These are used in ScriptMenu.cpp and kept separate from script.cpp to
 * keep the game loop and script loop clean and separate.
 */
void onMain();
void onExit();
void onRight();
void onLeft();
void update_menu();
