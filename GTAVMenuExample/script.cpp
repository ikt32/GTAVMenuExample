/*
 * This file is an example implementation of the menu system.
 */
#include "script.h"

// This include works from C++17 onwards, which is for filesystem access.
// I use it for demoing images, so if you use boost or some other way to
// explore folders, no need for this.
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

// You'll only need to include <menu.h> to start using it,
// after setting paths up properly.
#include <menu.h>

// You can ignore these includes as they're only used to resolve
// paths, log things to a file or to provide nice wrappers for natives.
#include "Util/Paths.h"
#include "Util/Util.hpp"
#include "Util/Logger.hpp"
#include "Util/Versions.h"

int textureBgId;
// filename, handle, width, height
std::vector<std::tuple<std::string, int, int, int>> textureHandles;

NativeMenu::Menu menu;
std::string settingsMenuFile;

Player player;
Ped playerPed;

// A few state variables
bool checkBoxStatus = false;
float someFloat = 0.0f;
int someInt = 0;
int intStep = 1;
int stringsPos = 0;
int stepChoice = 0;

int numberOfOptions = 16;

// Choice of step size to demonstrate variable precision display
std::vector<float> floatSteps = {	
	1.0f,
	0.1f, 
	0.01f, 
	0.001f, 
	0.0001f, 
	0.00001f,
	0.000001f,
};

// Random words to go through in the menu
std::vector<std::string> strings = {
	"Hello",
	"world!",
	"This",
	"ain't",
	"NativeUI",
};

/*
 * The settings may be read multiple times if needed.
 */
void init() {
	menu.ReadSettings();
	logger.Write("Settings read");
	logger.Write("Initialization finished");
}

/*
 * This simple function is executed when the menu opens.
 */
void onMain() {
	logger.Write("Menu was opened");
	menu.ReadSettings();
	textureHandles.clear();
	std::string imgPath = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\img";
	for (auto &file : fs::directory_iterator(imgPath)) {
		int width;
		int height;
		std::stringstream fileName;
		fileName << file;
		if (!GetPNGDimensions(fileName.str(), &width, &height))
			continue;

		int handle = createTexture(fileName.str().c_str());

		textureHandles.push_back(std::make_tuple(fileName.str(), handle, width, height));
	}
}

/*
 * This simple function is executed when the menu closes.
 */
void onExit() {
	logger.Write("Menu was closed");
}

/*
 * We'll attach these functions in an OptionPlus method.
 */
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
	/*
	 * Each tick, the controls are checked. If the key is hit to open
	 * or close the menu, the binded functions are called.
	 * This function has to be called for navigation to work.
	 */
	menu.CheckKeys();

	/*
	 *  You can define a menu like this. 
	 *  The main menu should always be called "mainmenu".
	 */ 
	if (menu.CurrentMenu("mainmenu")) {
		// The title is NOT optional.
		menu.Title("NativeMenu");
		menu.Subtitle("C++ menu showcase");

		// This is a normal option. It'll return true when "select" is presed.
		if (menu.Option("Click me!", { "This will log something to " + Paths::GetModuleNameWithoutExtension() + ".log" })) {
			showNotification("Check the logfile!");
			logger.Write("\"Click me!\" was selected!");
		}

		// This will open a submenu with the name "submenu"
		menu.MenuOption("Look, a submenu!", "submenu", { "This submenu demonstrates a few settings."});
		menu.MenuOption("Variable size demo", "varmenu", {"This submenu demonstrates how items can be added or removed dynamically."});
		menu.MenuOption("Title demo", "titlemenu", { "Showcase different title drawing options." });
		menu.MenuOption("Image demo", "imagemenu", { "Load images from a folder and show 'em all." });

		// Showing static information is also possible if a string vector only contains one element.
		int nothing = 0;
		menu.StringArray("Version", { DISPLAY_VERSION }, nothing, 
						 { "Thanks for checking out this menu!", "-ikt",  eGameVersionToString(getGameVersion())});
	}

	// Any submenus can have any titles. They should only need to match
	// the name used to call them.
	if (menu.CurrentMenu("submenu")) {
		menu.Title("I'm a submenu!");
		menu.Subtitle("Yay!");

		menu.BoolOption("Here's a checkbox", checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
		menu.IntOption("Ints!", someInt, -100, 100, intStep, { "Stepsize can be changed!" });
		menu.IntOption("Int step size", intStep, 1, 100, 1, { "Stepsize can be changed!" });
		menu.FloatOption("Floats?", someFloat, -100.0f, 100.0f, floatSteps[stepChoice], { "Try holding left/right, things should speed up." });
		menu.FloatArray("Float step size", floatSteps, stepChoice, { "Something something magic!" });
		menu.StringArray("String arrays", strings, stringsPos, { "You can also show different strings" });

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
		menu.OptionPlus("Look to the right!", extraInfo, std::bind(onLeft), std::bind(onRight), "Something", 
		{"You do need to manage the line splitting yourself, as it was intended for short pieces of info."});
	}

	if (menu.CurrentMenu("varmenu")) {
		menu.Title("Variable items");
		menu.Subtitle("Whoa!");

		menu.IntOption("Options #", numberOfOptions, 1, 999, 1, {"Increase or decrease the amount of items."});

		for (int i = 0; i < numberOfOptions; i++) {
			int display = i + 1;
			menu.IntOption("Option number", display, display, display, 0, { "This option was automatically generated." });
		}
	}

	if (menu.CurrentMenu("titlemenu")) {
		menu.Title("Title showcase");
		menu.Subtitle("MORE TITLES");

		menu.MenuOption("No subtitle", "nosubtitlemenu", { "Wanna see how the menu looks like without subtitle?" });
		menu.MenuOption("Los Santos Customs background", "title_lscmenu", { "Internal sprites as background." });
		menu.MenuOption("Custom background", "title_pngmenu", { "External textures as background." });
		menu.MenuOption("Long title text", "longtitlemenu",
		{ "Title too long? We'll fix it! Resizes and splits lines. Guaranteed fittage for anything up to 2 lines of text. "
			"Want longer? You probably shouldn't put entire poems in there ;)" });
	}

	if (menu.CurrentMenu("title_lscmenu")) {
		menu.Title("", "shopui_title_carmod", "shopui_title_carmod");
		menu.Subtitle("Fwoop");

		menu.Option("Dummy option");
	}

	if (menu.CurrentMenu("title_pngmenu")) {
		menu.Title("", textureBgId);
		menu.Subtitle("Custom title background!");

		menu.Option("Dummy option");
	}


	if (menu.CurrentMenu("nosubtitlemenu")) {
		menu.Title("No subtitle?");
		menu.Option("Normal option");
		std::vector<std::string> nope = {
			"Here",
			"are",
			"words"
		};
		menu.OptionPlus("OptionPlus ->", nope, nullptr, nullptr, "See?", { "See, it also disappears here." });
	}

	if (menu.CurrentMenu("longtitlemenu")) {
		menu.Title("Further Adventures in Finance and Felony");
		menu.Subtitle("Limited Title Splitting");
		menu.Option("Dummy option");
	}

	if (menu.CurrentMenu("imagemenu")) {
		menu.Title("Images");
		menu.Subtitle("Image showcase");

		for (auto handle : textureHandles) {
			fs::path p(std::get<0>(handle));

			std::vector<std::string> extras;
			//extras.push_back("Even mixes with text!");
			extras.push_back(menu.ImagePrefix + std::to_string(std::get<1>(handle)) + 
							 "W" + std::to_string(std::get<2>(handle)) +
							 "H" + std::to_string(std::get<3>(handle)));
			extras.push_back(p.filename().string());
			menu.OptionPlus(p.filename().string(), extras, nullptr, nullptr, "Image");
		}
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

	/* Whatever happens normally in your script! */
}

inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
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
