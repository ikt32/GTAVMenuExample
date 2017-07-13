#pragma once

#include <string>
#include <vector>

struct Color {
	int R;
	int G;
	int B;
	int A;
};

const Color solidWhite = {
	255, 255, 255, 255
};

// Natives called
void showText(float x, float y, float scale, const char* text, int font = 0, const Color &rgba = solidWhite, bool outline = false);
void showNotification(const char* message, int *prevNotification = nullptr);
void showSubtitle(std::string message, int duration = 2500);
std::string prettyNameFromHash(Hash hash);

//http://stackoverflow.com/questions/36789380/how-to-store-a-const-char-to-a-char
class CharAdapter
{
public:
	explicit CharAdapter(const char* s) : m_s(::_strdup(s)) { }
	CharAdapter(const CharAdapter& other) = delete; // non construction-copyable
	CharAdapter& operator=(const CharAdapter&) = delete; // non copyable
	
	~CharAdapter() /*free memory on destruction*/
	{
		::free(m_s); /*use free to release strdup memory*/
	}
	operator char*() /*implicit cast to char* */
	{
		return m_s;
	}

private:
	char* m_s;
};

bool GetIMGDimensions(std::string file, int *width, int *height);

bool GetPNGDimensions(std::string file, int *width, int *height);
bool GetJPGDimensions(std::string file, int *width, int *height);
