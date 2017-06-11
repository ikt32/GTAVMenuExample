#include "settings.h"
#include "menucontrols.h"
#include "simpleini/SimpleIni.h"
#include "keyboard.h"
#include <vector>
#include "menu.h"
#include <fstream>

Settings::Settings() { }


Settings::~Settings() { }

void Settings::SetFiles(const std::string &general, const std::string &menu) {
	settingsMenuFile = menu;
}

void Settings::ReadSettings(NativeMenu::MenuControls *control, NativeMenu::Menu *menuOpts) {
	CSimpleIniA settingsMenu;
	settingsMenu.SetUnicode();
	settingsMenu.LoadFile(settingsMenuFile.c_str());
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuKey]	= str2key(settingsMenu.GetValue("MENU", "MenuKey",		"VK_OEM_4"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuUp]		= str2key(settingsMenu.GetValue("MENU", "MenuUp",		"UP"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuDown]	= str2key(settingsMenu.GetValue("MENU", "MenuDown",		"DOWN"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuLeft]	= str2key(settingsMenu.GetValue("MENU", "MenuLeft",		"LEFT"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuRight]	= str2key(settingsMenu.GetValue("MENU", "MenuRight",	"RIGHT"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuSelect] = str2key(settingsMenu.GetValue("MENU", "MenuSelect",	"RETURN"));
	control->ControlKeys[NativeMenu::MenuControls::ControlType::MenuCancel] = str2key(settingsMenu.GetValue("MENU", "MenuCancel",	"BACKSPACE"));
#pragma warning(push)
#pragma warning(disable: 4244)
	menuOpts->menux = settingsMenu.GetDoubleValue("MENU", "MenuX", 0.2);
	menuOpts->menuy = settingsMenu.GetDoubleValue("MENU", "MenuY", 0.125);
#pragma warning(pop)
}

void Settings::SaveSettings() {

}
