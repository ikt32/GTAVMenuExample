/*
 * This file is an example implementation of the menu system.
 */
#include "script.h"

// You'll only need to include <menu.h> to start using it,
// after setting paths up properly.
#include <menu.h>

// You can ignore these includes as they're only used to resolve
// paths, log things to a file or to provide nice wrappers for natives.
#include "Util/Paths.h"
#include "Util/Util.hpp"
#include "Util/Logger.hpp"

int textureBgId;
int textureBgId2;

NativeMenu::Menu menu;
std::string settingsMenuFile;

Player player;
Ped playerPed;

/*
 * The settings may be read multiple times if needed.
 */
void init() {
	menu.ReadSettings();
}

void update_game() {
	/* 
	 * Whatever happens normally in your script! 
	 * For new, let's just update the player ped.
	 */
    player = PLAYER::PLAYER_ID();
    playerPed = PLAYER::PLAYER_PED_ID();
}

void main() {
	logger.Write("Script started");

	// Check the paths on runtime, though this could also be hardcoded with a relative path.
	settingsMenuFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\settings_menu.ini";
	menu.SetFiles(settingsMenuFile);
	logger.Write("Loading " + settingsMenuFile);

	// Create the custom background texture. It's required to manually check if the file exists, otherwise ScriptHookV crashes!
	std::string textureBgFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\img\\custom_background.png";
	if (exists(textureBgFile)) {
		textureBgId = createTexture(textureBgFile.c_str());
	}
	else {
		logger.Write("ERROR: " + textureBgFile + " does not exist.");
	}
	std::string textureBg2File = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\img\\custom_background2.png";
	if (exists(textureBg2File)) {
		textureBgId2 = createTexture(textureBg2File.c_str());
	}
	else {
		logger.Write("ERROR: " + textureBg2File + " does not exist.");
	}

	// Register callbacks for menu entry and exit.
	menu.RegisterOnMain(std::bind(onMain));
	menu.RegisterOnExit(std::bind(onExit));

	init();

	while (true) {
		update_game();
		update_menu();
		WAIT(0);
	}
}

void ScriptMain() {
	srand(GetTickCount());
	main();
}
