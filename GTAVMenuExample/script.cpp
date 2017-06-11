#include "script.h"

#include <sstream>
#include <menu.h>
#include <menucontrols.h>

#include "Util/Paths.h"
#include "Util/Util.hpp"
#include "Util/Logger.hpp"

#include "settings.h"

std::string prettyNameFromHash(Hash hash) {
	char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
	std::string displayName = UI::_GET_LABEL_TEXT(name);
	if (displayName == "NULL") {
		displayName = name;
	}
	return displayName;
}

Player player;
Ped playerPed;
Vehicle vehicle;
Hash model;

Settings settings;

NativeMenu::Menu menu;
NativeMenu::MenuControls controls;
std::string settingsMenuFile;

bool checkBoxStatus = false;
float someFloat = 0.0f;
float floatStep = 0.01f;
int someInt = 0;
int intStep = 1;

int stringsPos = 0;
std::vector<std::string> strings = {
	"Here",
	"are",
	"a",
	"few",
	"words",
	"to",
	"choose",
	"from",
};

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

void onRight() {
	showNotification("You pressed RIGHT on an OptionPlus");
}

void onLeft() {
	showNotification("You pressed LEFT on an OptionPlus");
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
		menu.Title("Example!");

		// This will open a submenu with the name "submenu"
		menu.MenuOption("Look, a submenu!", "submenu", { "This submenu demonstrates a few settings."});
	}

	// Any submenus can have any titles. They should only need to match
	// the name used to call them.
	if (menu.CurrentMenu("submenu")) {
		menu.Title("I'm a submenu!");

		if (menu.Option("Click me!", { "This will log something to " + Paths::GetModuleNameWithoutExtension() + ".log" })) {
			showNotification("Check the logfile!");
			logger.Write("\"Click me!\" was selected!");
		}

		menu.BoolOption("Here's a checkbox", &checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
		menu.IntOption("Ints!", &someInt, -100, 100, intStep, { "Stepsize can be changed!" });
		menu.IntOption("Int step size", &intStep, 1, 100, 1, { "Stepsize can be changed!" });
		menu.FloatOption("Floats?", &someFloat, -100.0f, 100.0f, floatStep, { "Try holding left/right, things should speed up." });
		menu.FloatOption("Float step size", &floatStep, 0.01f, 100.0f, 0.01f, { "Steps smaller than 0.01 aren't visible until they go over the 0.01 mark." });
		menu.StringArray("String arrays", strings, &stringsPos, { "You can also show different strings" });

		menu.Option("Description info",
		{ "You can put arbitarily long texts in the description. "
		"Word wrapping magic should work! "
		"That's why this subtext is so big ;)",
		"Newlines",
		"like so."});

		// Some extra information can be shown on the right of the the menu.
		// You do need to manage newlines yourself.
		std::vector<std::string> extraInfo = {
			"There's also some additional info",
			"You can put descriptions or info here",
			"Each string is a new line",
			"The box expands by itself"
		};
		menu.OptionPlus("Look to the right!", extraInfo, nullptr, std::bind(onLeft), std::bind(onRight), "Something", 
		{"You do need to manage the line splitting yourself, as it's meant for short pieces of info."});
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
