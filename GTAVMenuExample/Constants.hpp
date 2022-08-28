#pragma once
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 2
#define VERSION_MINOR 0
#define VERSION_PATCH 0

namespace Constants {
    static const char* const ScriptName = "Menu Example";
    static const char* const NotificationPrefix = "~b~Menu Example~w~";
    static const char* const DisplayVersion = "v" STR(VERSION_MAJOR) "."  STR(VERSION_MINOR) "." STR(VERSION_PATCH);

    // This can probably be renamed to "authorFolder"
    // This is to create %localappdata%/ikt/GTAVMenuExample if for some reason
    // the GTA V folder is not read/writeable for the script.
    static const char* const iktFolder = "ikt";
    static const char* const ScriptFolder = "GTAVMenuExample";
}
