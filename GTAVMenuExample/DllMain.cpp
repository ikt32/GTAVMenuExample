/*
 * DllMain.cpp hosts the DllMain function, which is the entry point for the DLL.
 * Initialization happens here.
 * In this example, it initializes the logger and paths, and registers the main
 * script function to ScriptHookV using scriptRegister.
 */

#include "Script.hpp"
#include "Constants.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"

#include <inc/main.h>
#include <filesystem>

namespace fs = std::filesystem;

/*
 * As the game is often installed in a default directory, like
 * C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto V
 * Windows permissions may not allow scripts to write there.
 * This function detects if that happens and copies the files it needs to
 * %localappdata%\{author}\{scriptname}.
 * 
 * Use Paths::GetModPath() to ensure the relocated path is used, instead of the
 * GTA V directory.
 */
void initializePaths(HMODULE hInstance) {
    Paths::SetOurModuleHandle(hInstance);

    auto localAppDataPath = Paths::GetLocalAppDataPath();
    auto localAppDataModPath = localAppDataPath / Constants::iktFolder / Constants::ScriptFolder;
    auto originalModPath = Paths::GetModuleFolder(hInstance) / Constants::ScriptFolder;
    Paths::SetModPath(originalModPath);

    bool useAltModPath = false;
    if (fs::exists(localAppDataModPath)) {
        useAltModPath = true;
    }

    fs::path modPath;
    fs::path logFile;

    // Use LocalAppData if it already exists.
    if (useAltModPath) {
        modPath = localAppDataModPath;
        logFile = localAppDataModPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
    }
    else {
        modPath = originalModPath;
        logFile = modPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
    }

    Paths::SetModPath(modPath);

    if (!fs::is_directory(modPath) || !fs::exists(modPath)) {
        fs::create_directories(modPath);
    }

    g_Logger.SetFile(logFile.string());
    g_Logger.Clear();

    if (g_Logger.Error()) {
        modPath = localAppDataModPath;
        logFile = localAppDataModPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
        fs::create_directories(modPath);

        Paths::SetModPath(modPath);
        g_Logger.SetFile(logFile.string());

        fs::copy(originalModPath, localAppDataModPath,
            fs::copy_options::update_existing | fs::copy_options::recursive);

        std::vector<std::string> messages;

        // Fix perms
        for (auto& path : fs::recursive_directory_iterator(localAppDataModPath)) {
            try {
                fs::permissions(path, fs::perms::all);
            }
            catch (std::exception& e) {
                messages.push_back(
                    std::format("Failed to set permissions on [{}]: {}",
                        path.path().string(), e.what()));
            }
        }

        g_Logger.ClearError();
        g_Logger.Clear();

        LOG(WARN, "Copied to [{}] from [{}] due to read/write issues.",
            modPath.string(), originalModPath.string());

        if (!messages.empty()) {
            LOG(WARN, "Encountered issues while updating permissions:");
            for (const auto& message : messages) {
                LOG(WARN, "{}", message);
            }
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            g_Logger.SetMinLevel(DEBUG);
            initializePaths(hInstance);
            LOG(INFO, "{} {} (built {} {})", Constants::ScriptName, Constants::DisplayVersion, __DATE__, __TIME__);
            LOG(INFO, "Data path: {}", Paths::GetModPath().string());

            scriptRegister(hInstance, MenuExample::ScriptMain);
            LOG(INFO, "Script registered");
            break;
        }
        case DLL_PROCESS_DETACH: {
            scriptUnregister(hInstance);
            break;
        }
        default: {
            break;
        }
    }
    return TRUE;
}
