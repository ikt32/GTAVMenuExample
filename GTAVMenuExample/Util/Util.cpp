#define NOMINMAX
#include "../../../ScriptHookV_SDK/inc/natives.h"
#include "Util.hpp"
#include <algorithm>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void showText(float x, float y, float scale, const char* text, int font, const Color &rgba, bool outline) {
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(rgba.R, rgba.G, rgba.B, rgba.A);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	if (outline) UI::SET_TEXT_OUTLINE();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(text));
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}

void showNotification(const char* message, int *prevNotification) {
	if (prevNotification != nullptr && *prevNotification != 0) {
		UI::_REMOVE_NOTIFICATION(*prevNotification);
	}
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");

	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(message));
	
	int id = UI::_DRAW_NOTIFICATION(false, false);
	if (prevNotification != nullptr) {
		*prevNotification = id;
	}
}

// borrowed from FiveM <3
void showSubtitle(std::string message, int duration) {
	UI::BEGIN_TEXT_COMMAND_PRINT("CELL_EMAIL_BCON");

	const int maxStringLength = 99;

	for (int i = 0; i < message.size(); i += maxStringLength) {
		int npos = std::min(maxStringLength, static_cast<int>(message.size()) - i);
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(message.substr(i, npos).c_str()));
	}

	UI::END_TEXT_COMMAND_PRINT(duration, 1);
}

std::string prettyNameFromHash(Hash hash) {
	char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
	std::string displayName = UI::_GET_LABEL_TEXT(name);
	if (displayName == "NULL") {
		displayName = name;
	}
	return displayName;
}

// https://codereview.stackexchange.com/questions/149717/implementation-of-c-standard-library-function-ntohl
uint32_t my_ntohl(uint32_t const net) {
	uint8_t data[4] = {};
	memcpy(&data, &net, sizeof(data));

	return ((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24);
}

bool GetPNGDimensions(std::string file, int *width, int *height) {
	if (fs::path(file).extension() != ".png")
		return false;

	std::ifstream in(file);
	int _width, _height;

	in.seekg(16);
	in.read((char *)&_width, 4);
	in.read((char *)&_height, 4);

	*width = my_ntohl(_width);
	*height = my_ntohl(_height);

	return true;
}
