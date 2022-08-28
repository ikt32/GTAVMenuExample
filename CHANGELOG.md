# Changelog

Commits should be fine to read, but this is here anyway for the larger changes that need more than 50 characters to explain.

## 2.0

Basically a full rewrite, based on how my new scripts use the menu and their general structure.

The underlaying menu is still one and the same horrible mess, but the pretty wrapper around it makes
usage a lot more user (or, developer: me)-friendly. For one, gone are the days where one needs to
define a submenu name, a function that implements it, and the call to run it, while also calling
the required methods. It's all in the wrapper now.

The script instance that's passed there allows the menu to interact with the core mod logic
in a defined manner (public methods), so no more throwing around globals and loose functions.

Loose functions still exist in the "called by DllMain stuff", but that should be mostly limited to
initialization and grabbing the script instance handles. Oh yeah, splitting things up allows
to create multiple instances, for example a "script" that works for all NPCs.

## 1.0

Initial publish. Based on the proof-ish of concept made for VStancer to have *some* UI, as before
that my scripts were just .ini based and the user was expected to dig around in there using Notepad.
