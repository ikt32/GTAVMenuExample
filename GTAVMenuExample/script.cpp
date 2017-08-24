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
int textureBgId2;

class MenuTexture {
public:
	MenuTexture(): Handle(-1),
	             Width(480),
	             Height(270) { }

	MenuTexture(std::string filename, int handle, int width, int height) :
		    Filename(filename), Handle(handle), Width(width), Height(height) { }

	std::string Filename;
	int Handle;
	int Width;
	int Height;
};

std::vector<MenuTexture> g_textures;

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
int prevNotification;

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
	"Hello, ",
	"world!",
};

/*
 * Some utilities
 */
bool isFileAdded(std::string filename, std::vector<MenuTexture> things) {
	return std::find_if(things.begin(), things.end(), [&filename](const MenuTexture& element) { return element.Filename == filename; }) != things.end();
}

inline bool exists(const std::string& name) {
	struct stat buffer;
	return stat(name.c_str(), &buffer) == 0;
}

/*
 * Only scan for added images.
 */
void scanImageFolder() {
	std::string imgPath = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\img";
	for (auto &file : fs::directory_iterator(imgPath)) {
		std::string fileName = fs::path(file).string();
		if (isFileAdded(fileName, g_textures)) {
			continue;
		}
		
		int width;
		int height;
		if (!GetIMGDimensions(fileName, &width, &height))
			continue;

		int handle = createTexture(fileName.c_str());

		g_textures.push_back(MenuTexture(fileName, handle, width, height));
	}
	std::vector<std::vector<MenuTexture>::iterator> thingsToErase;
	for (auto texture : g_textures) {
		if (!exists(texture.Filename)) {
			thingsToErase.push_back(std::find_if(g_textures.begin(), g_textures.end(), [&texture](const MenuTexture& element) { return element.Filename == texture.Filename; }));
		}
	}
	for (auto eraseMe : thingsToErase) {
		g_textures.erase(eraseMe);
	}
}

/*
* The settings may be read multiple times if needed.
*/
void init() {
	menu.ReadSettings();
}

/*
 * This simple function is executed when the menu opens.
 */
void onMain() {
	menu.ReadSettings();
	scanImageFolder();
}

/*
 * This simple function is executed when the menu closes.
 */
void onExit() {
	showNotification("Menu was closed", &prevNotification);
}

/*
 * We'll attach these functions in an OptionPlus method.
 */
void onRight() {
	showNotification("You pressed RIGHT on an OptionPlus", &prevNotification);
}

void onLeft() {
	showNotification("You pressed LEFT on an OptionPlus", &prevNotification);
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
		menu.Title("Menu Example");
		menu.Subtitle("~r~C++ ~b~MENU SHOWCASE");

		// This is a normal option. It'll return true when "select" is presed.
		if (menu.Option("Click me!", { "This will log something to " + Paths::GetModuleNameWithoutExtension() + ".log" })) {
			showNotification("Check the logfile!", &prevNotification);
			logger.Write("\"Click me!\" was selected!");
		}

		// This will open a submenu with the name "submenu"
		menu.MenuOption("Option demos", "submenu", { "This submenu demonstrates usage of some options."});
		menu.MenuOption("Variable size demo", "varmenu", {"This submenu demonstrates how items can be added or removed dynamically."});
		menu.MenuOption("Title demo", "titlemenu", { "Showcase different title drawing options." });
		menu.MenuOption("Image demo", "imagemenu", { "Showcase OptionPlus image arguments" });

		// Showing static information is also possible if a string vector only contains one element.
		int nothing = 0;
		menu.StringArray("Version", { DISPLAY_VERSION }, nothing, 
						 { "Thanks for checking out this menu!", "-ikt",  eGameVersionToString(getGameVersion())});
	}

	// Any submenus can have any titles. They should only need to match
	// the name used to call them.
	if (menu.CurrentMenu("submenu")) {
		menu.Title("Option demos");
		menu.Subtitle("");

		menu.BoolOption("BoolOption", checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
		menu.IntOption("IntOption step size", intStep, 1, 100, 1, { "Change the step size for IntOption below" });
		menu.IntOption("IntOption", someInt, -100, 100, intStep, { "Stepsize: " + std::to_string(intStep) });
		menu.FloatArray("FloatOption step size", floatSteps, stepChoice, { "Change the step size for FloatOption below.",
																		   "Note: this option uses FloatArray."});
		menu.FloatOption("FloatOption", someFloat, -100.0f, 100.0f, floatSteps[stepChoice], { "Try holding left/right, things should speed up." });
		menu.StringArray("StringArray", strings, stringsPos, { "You can also show different strings" });

		menu.Option("Option/Details",
		{ "You can put arbitarily long texts in the details. Word wrapping happens automatically.",
		  "Vector elements are newlines."});

		// Some extra information can be shown on the right of the the menu.
		std::vector<std::string> extraInfo = {
			"OptionPlus",
			"This box supports images, in-game sprites and texts. "
			"Longer texts can be used without problems, this box should split the lines "
			"by itself. As with the details, a new vector element inserts a newline.",
			"See?"
		};
		menu.OptionPlus("OptionPlus", extraInfo, nullptr, std::bind(onRight), std::bind(onLeft), "OptionPlus",
		{ "This box also manages string splitting for width." });
	}

	if (menu.CurrentMenu("varmenu")) {
		menu.Title("Variable size");
		menu.Subtitle("");

		menu.IntOption("Options #", numberOfOptions, 1, 999, 1, {"Increase or decrease the amount of items."});

		for (int i = 0; i < numberOfOptions; i++) {
			int display = i + 1;
			menu.IntOption("Option number", display, display, display, 0, { "This option was automatically generated." });
		}
	}

	if (menu.CurrentMenu("titlemenu")) {
		menu.Title("Title demo");
		menu.Subtitle("");

		menu.MenuOption("Los Santos Customs background", "title_lscmenu", { "Internal sprites as background." });
		menu.MenuOption("Custom background", "title_pngmenu", { "External textures as background." });
		menu.MenuOption("Custom background 2", "title_pngmenu2", { "External textures as background. Colored footer." });

		menu.MenuOption("Long title text", "longtitlemenu",
		{ "Automatically fit and resize long titles. Works for up to 2 lines of text (after processing)." });
	}

	if (menu.CurrentMenu("title_lscmenu")) {
		menu.Title("", "shopui_title_carmod", "shopui_title_carmod");
		menu.Subtitle("Sprite background");
		menu.Option("Option");
	}

	if (menu.CurrentMenu("title_pngmenu")) {
		menu.Title("", textureBgId);
		menu.Subtitle("Custom background");
		menu.Option("Option");
	}

	if (menu.CurrentMenu("title_pngmenu2")) {
		menu.Title("", textureBgId2);
		menu.Subtitle("Custom background");
		menu.Option("Option");
		menu.Footer({173, 236, 173, 255});
	}

	if (menu.CurrentMenu("longtitlemenu")) {
		menu.Title("Further Adventures in Finance and Felony");
		menu.Subtitle("Long title text");
		menu.Option("Option");
	}

	if (menu.CurrentMenu("imagemenu")) {
		menu.Title("Images");
		menu.Subtitle("Image demo");

		{
			std::vector<std::string> extras;
			extras.push_back(menu.SpritePrefix +
				"dock_default" + " " +
				"seashark" + " " +
				"W" + std::to_string(512) +
				"H" + std::to_string(256));
			menu.OptionPlus("Game texture", extras, nullptr, nullptr, nullptr, "Sprite");
		}

		for (auto texture : g_textures) {
			fs::path p(texture.Filename);
			std::vector<std::string> extras;
			extras.push_back(menu.ImagePrefix + std::to_string(texture.Handle) + 
							 "W" + std::to_string(texture.Width) +
							 "H" + std::to_string(texture.Height));
			extras.push_back(p.filename().string());
			menu.OptionPlus(p.filename().string(), extras, nullptr, nullptr, nullptr, "Image");
		}
	}

	// Finally, draw all textures.
	menu.EndMenu();
}

void update_game() {
	/* Whatever happens normally in your script! */
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
