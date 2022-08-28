#pragma once

#include <map>
#include <string>

/*
 * To demonstrate the menu image stuff, a struct with
 * some image information is made.
 * The menu requires the texture handle ScriptHookV generates for the image itself,
 * and the width and height to properly set the aspect ratio.
 */
struct SMenuTexture {
    SMenuTexture(const std::string& filename, int handle, int width, int height)
        : Filename(filename)
        , Handle(handle)
        , Width(width)
        , Height(height) {
    }

    std::string Filename;
    int Handle = -1;
    int Width = 480;
    int Height = 270;
};

namespace MenuTexture {
    const std::map<std::string, SMenuTexture>& GetTextures();
    void UpdateTextures();
}
