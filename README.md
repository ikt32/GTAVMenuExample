# GTAVMenuExample

[![Build status](https://ci.appveyor.com/api/projects/status/2ttmwbmmt1vxamoi?svg=true)](https://ci.appveyor.com/project/E66666666/gtavmenuexample)

Example script of how to use [GTAVMenuBase](https://github.com/E66666666/GTAVMenuBase).

![Comparison](MenuCompare.png)

## Building

* [ScriptHookV SDK by Alexander Blade](http://www.dev-c.com/gtav/scripthookv/)
* [GTAVMenuBase](https://github.com/E66666666/GTAVMenuBase)
* [simpleini](https://github.com/brofield/simpleini)

GTAVMenuBase and simpleini are added as submodule in the `thirdparty` directory.
Note that GTAVMenuBase depends on simpleini (but doesn't do anything to get it, so your project has to)
and expects the directory containing the `simpleini/` directory to be in the include directories,
such that `#include <simpleini/SimpleIni.h>` works.

Similarly, it expects the `ScriptHookV_SDK/` directory to be in the include directories,
such that `#include <inc/natives.h>` works.

The ScriptHookV SDK is in the project repository itself, in `thirdparty/ScriptHookV_SDK`.
The `natives.h` files is generated by [Dot.'s Native DB](https://nativedb.dotindustries.dev/natives),
and is newer than the one included by Alexander Blades' SDK with corrected namespaces.

Once cloned, the project should just build with Visual Studio 2022.

## Usage

v2.0.0 of the example is completely rewritten to how my newer scripts use it. A few general concepts:

* Generally, things should be more neatly divided and be less of a major clusterfuck.
* `CScriptMenu` wraps the `NativeMenu::Menu` and calls boilerplate stuff.
* `CScriptMenu::CSubmenu` represents a single "menu page".
* There is a "script instance" object that can be passed for the menu to directly operate on.

I'd recommend the following order to get a general grasp of how this thing works:

1. `DllMain.cpp` - Dll entrypoint and some setup stuff
2. `Script.cpp/hpp` - Script initialization
3. `MenuExampleScript.cpp/hpp` - The 'core' logic of the 'script' part which interacts with the game world.
4. `MenuExampleMenu.cpp` - The menu implementation.

Plenty of comments have been left in the source code files to figure out what the heck everything actually does.

Feel free to use this as a base project, it lends itself pretty well for most purposes and can be easily
extended for multiple instances (for NPCs, etc.).

\- ikt
