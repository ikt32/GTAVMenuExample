#include "script.h"

#include <sstream>
#include <menu.h>
#include <menucontrols.h>

#include "Util/Paths.h"
#include "Util/Util.hpp"
#include "Util/Logger.hpp"

#include "settings.h"

Player player;
Ped playerPed;
Vehicle vehicle;
Hash model;

Settings settings;

NativeMenu::Menu menu;
NativeMenu::MenuControls controls;
std::string settingsMenuFile;

void init() {
	// TODO: Separately reading in files is a bit dirty, this'll need some work.
	settings.ReadSettings(&controls, &menu);
	menu.LoadMenuTheme(std::wstring(settingsMenuFile.begin(), settingsMenuFile.end()).c_str());
	logger.Write("Settings read");
	logger.Write("Initialization finished");
}

/*
 * This simple function is executed when the menu opens.
 */
void onMain() {
	logger.Write("Menu was opened");
}

/*
 * This simple function is executed when the menu closes. You can handle things
 * you temporarily stored in the menu, for example.
 */
void onExit() {
	logger.Write("Menu was closed");
}

/*
 * update_menu() should be called each tick.
 */

void update_menu() {
	// Each tick, the controls are checked. If the key is hit to open
	// or close the menu, the binded functions are called.
	menu.CheckKeys(&controls, std::bind(onMain), std::bind(onExit));

	// You can define a menu like this. The main menu should always be
	// called "mainmenu".
	if (menu.CurrentMenu("mainmenu")) {
		// The title is NOT optional.
		menu.Title("Whoopie!");

		if (menu.Option("Click me!")) {
			logger.Write("Option was chosen");
		}
		// This will open a submenu with the name "submenu"
		menu.MenuOption("Look, a submenu!", "submenu");
	}

	// Any submenus can have any titles. They should only need to match
	// the name used to call them.
	if (menu.CurrentMenu("submenu")) {
		menu.Title("I'm a submenu!");

		// Some extra information can be shown on the right of the the menu.
		std::vector<std::string> extraInfo = {
			"There's also some additional info! You can put descriptions"
			" or info here. This automatically splits the lines so they"
			" fit in the menu."
		};
		menu.OptionPlus("Look to the right!", extraInfo);
	}

	// Finally, draw all textures.
	menu.EndMenu();
}

void update_game() {
	player = PLAYER::PLAYER_ID();
	playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player)) {
		menu.CloseMenu();
		return;
	}

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE)) {
		menu.CloseMenu();
		return;
	}

	vehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	model = ENTITY::GET_ENTITY_MODEL(vehicle);
}

void main() {
	logger.Write("Script started");

	settingsMenuFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\settings_menu.ini";
	settings.SetFiles("", settingsMenuFile);

	logger.Write("Loading " + settingsMenuFile);

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
