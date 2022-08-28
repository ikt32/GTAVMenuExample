#pragma once

#include <menu.h>
#include <string>

/*
 * This class calls boilerplate functions of the menu.
 * The main script only needs to call Tick() to fully update the menu.
 * This way the menu can be used in a more OOP way than the original
 * design (or lack thereof) initially allowed.
 */
template <typename T>
class CScriptMenu
{
public:
    /*
     * The Submenu class represents an actual "menu": e.g. the main menu is also a "sub"-menu.
     * Within a Submenu body, the NativeMenu object functions (Title, Subtitle, Option, etc)
     * can be called from the "mbCtx" (menubase context).
     * Using the scriptContext, public methods of the script can be called - to retrieve the
     * player ped, or whatever else is needed for the menu to interact with it.
     */
    class CSubmenu {
    public:
        // Submenu added by name, useful for most purposes (as submenu).
        CSubmenu(std::string name,
            std::function<void(NativeMenu::Menu&, T&)> menuBody)
            : mName(std::move(name))
            , mBody(std::move(menuBody))
            , mPred(nullptr) {
        }

        // Submenu added by predicate, useful for interaction-ish activation,
        // e.g. game context: When near a car, set predicate to true.
        CSubmenu(std::function<bool(NativeMenu::Menu&)> predicate,
            std::function<void(NativeMenu::Menu&, T&)> menuBody)
            : mName() // unused
            , mBody(std::move(menuBody))
            , mPred(std::move(predicate)) {
        }

        // Called by CSubmenu for all submenus
        void Update(NativeMenu::Menu& mbCtx, T& scriptContext) {
            if (mPred && mPred(mbCtx)) {
                mBody(mbCtx, scriptContext);
                return;
            }

            if (mbCtx.CurrentMenu(mName))
                mBody(mbCtx, scriptContext);
        }
    private:
        std::string mName;
        std::function<void(NativeMenu::Menu&, T&)> mBody;
        std::function<bool(NativeMenu::Menu&)> mPred;
    };

    // The actual menu. Has its own settings and
    // can call stuff when opened or closed.
    CScriptMenu(std::string settingsFile,
        std::function<void()> onInit,
        std::function<void()> onExit,
        std::vector<CSubmenu> submenus)
        : mSettingsFile(std::move(settingsFile))
        , mSubmenus(std::move(submenus)) {
        mMenuBase.RegisterOnMain([this, onInit]() {
            mMenuBase.ReadSettings();
            onInit();
        });
        mMenuBase.RegisterOnExit(std::move(onExit));
        mMenuBase.SetFiles(mSettingsFile);
        mMenuBase.Initialize();
        mMenuBase.ReadSettings();
    }

    // Call Tick() for the menu instance every game tick.
    void Tick(T& scriptContext) {
        mMenuBase.CheckKeys();

        for (auto& submenu : mSubmenus) {
            submenu.Update(mMenuBase, scriptContext);
        }

        mMenuBase.EndMenu();
    }

private:
    std::string mSettingsFile;
    NativeMenu::Menu mMenuBase;
    std::vector<CSubmenu> mSubmenus;
};
