"""Create the minimal cookable Pier Seven entry level for Shadowfront.

Run from the project root after UnrealEditor-Cmd exists:
UnrealEditor-Cmd Shadowfront.uproject -run=pythonscript -script=Content/Python/create_pier_seven_map.py -unattended -nop4

Gameplay geometry, lights, fog and hostile waves continue to be created by the
native Shadowfront game mode. This map establishes the cookable `/Game` level,
player start, world settings, and an intentional camera-safe spawn point.
"""

import unreal


MAP_PATH = "/Game/Maps/PierSeven"
MAP_ASSET_PATH = "/Game/Maps/PierSeven.PierSeven"
GAME_MODE_PATH = "/Script/Shadowfront.ShadowfrontGameMode"


def main() -> None:
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_ASSET_PATH):
        unreal.log(f"[Shadowfront Map] Keeping existing map: {MAP_PATH}")
        return

    unreal.EditorLevelLibrary.new_level(MAP_PATH)
    player_start = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.PlayerStart,
        unreal.Vector(0.0, 0.0, 12.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    player_start.set_actor_label("SF_PlayerStart_PierSeven")

    world = unreal.EditorLevelLibrary.get_editor_world()
    world_settings = world.get_world_settings()
    game_mode_class = unreal.load_class(None, GAME_MODE_PATH)
    if game_mode_class:
        world_settings.set_editor_property("default_game_mode", game_mode_class)
    else:
        unreal.log_warning(f"[Shadowfront Map] Could not resolve game mode class: {GAME_MODE_PATH}")

    unreal.EditorLevelLibrary.save_current_level()
    unreal.log("[Shadowfront Map] Created /Game/Maps/PierSeven with a Shadowfront player start.")


if __name__ == "__main__":
    main()
