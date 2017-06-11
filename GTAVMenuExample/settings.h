#pragma once
#include <vector>

namespace NativeMenu {
	class MenuControls;
	class Menu;
}
class Settings
{
public:
	Settings();
	~Settings();
	void ReadSettings(NativeMenu::MenuControls *control, NativeMenu::Menu *menuOpts);
	void SaveSettings();
	void SetFiles(const std::string &general, const std::string &menu);

private:
	std::string settingsMenuFile;
};
