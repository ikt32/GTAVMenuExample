#include "MenuExampleScript.hpp"

#include <inc/natives.h>
#include <format>

void CMenuExampleScript::Tick() {
    // Core script logic
    mDistance += ENTITY::GET_ENTITY_SPEED(PLAYER::PLAYER_PED_ID()) * MISC::GET_FRAME_TIME();
}

std::string CMenuExampleScript::GetPlayerHealth() {
    int health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());

    return std::format("{}", health);
}

std::string CMenuExampleScript::GetPlayerVehicleName() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        Hash model = ENTITY::GET_ENTITY_MODEL(playerVehicle);
        std::string makeName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(model));
        std::string modelName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));
        return std::format("{} {}", makeName, modelName);
    }
    else {
        return "No vehicle";
    }
}

std::string CMenuExampleScript::GetDistanceTraveled() {
    return std::format("{:.0f} meters", mDistance);
}

bool CMenuExampleScript::IsPlayerInVehicle() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    return ENTITY::DOES_ENTITY_EXIST(playerVehicle);
}

void CMenuExampleScript::ChangePlayerVehicleColor() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        VEHICLE::SET_VEHICLE_COLOUR_COMBINATION(playerVehicle,
            MISC::GET_RANDOM_INT_IN_RANGE(0,
                VEHICLE::GET_NUMBER_OF_VEHICLE_COLOURS(playerVehicle)
            )
        );
    }
}
