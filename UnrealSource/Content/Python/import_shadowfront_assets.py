"""Shadowfront asset import utility.

Run from the Unreal project root after UnrealEditor-Cmd is available:
UnrealEditor-Cmd Shadowfront.uproject -run=pythonscript -script=Content/Python/import_shadowfront_assets.py -unattended -nop4

The source assets intentionally remain in ExternalAssets.  This script copies them
into Unreal's asset database under /Game, where cooking can include them in an APK.
It is idempotent: existing assets with the same destination name are preserved.
"""

import os
import re
import unreal


PROJECT_DIR = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir())
EXTERNAL_ASSET_DIR = os.path.join(PROJECT_DIR, "ExternalAssets")

IMPORT_MANIFEST = (
    ("", "shadowfront-wet-concrete-material.png", "/Game/Art/Textures"),
    ("", "shadowfront-rusted-steel-material.png", "/Game/Art/Textures"),
    ("", "shadowfront-visor-mark.png", "/Game/Art/Textures"),
    ("Audio", "shadowfront-opening-theme.wav", "/Game/Audio/Music"),
    ("Audio", "shadowfront-combat-loop.wav", "/Game/Audio/Music"),
    ("Audio", "shadowfront-exploration-bed.wav", "/Game/Audio/Music"),
    ("Audio", "shadowfront-opening-comms.wav", "/Game/Audio/Dialogue"),
    ("Audio", "shadowfront-marwan-barks.wav", "/Game/Audio/Dialogue"),
    ("Audio", "shadowfront-silent-warden.wav", "/Game/Audio/Dialogue"),
)


def asset_name(file_name: str) -> str:
    source_name = os.path.splitext(file_name)[0]
    return re.sub(r"[^A-Za-z0-9_]", "_", source_name)


def destination_object_path(destination_dir: str, file_name: str) -> str:
    name = asset_name(file_name)
    return f"{destination_dir}/{name}.{name}"


def import_one(relative_source_dir: str, file_name: str, destination_dir: str) -> bool:
    source_path = os.path.join(EXTERNAL_ASSET_DIR, relative_source_dir, file_name)
    object_path = destination_object_path(destination_dir, file_name)

    if not os.path.isfile(source_path):
        unreal.log_error(f"[Shadowfront Import] Missing source asset: {source_path}")
        return False

    if unreal.EditorAssetLibrary.does_asset_exist(object_path):
        unreal.log(f"[Shadowfront Import] Keeping existing asset: {object_path}")
        return True

    task = unreal.AssetImportTask()
    task.filename = source_path
    task.destination_path = destination_dir
    task.destination_name = asset_name(file_name)
    task.automated = True
    task.replace_existing = False
    task.save = True

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    imported = task.imported_object_paths
    if not imported:
        unreal.log_error(f"[Shadowfront Import] Unreal did not import: {source_path}")
        return False

    unreal.log(f"[Shadowfront Import] Imported {source_path} -> {imported[0]}")
    return True


def main() -> None:
    unreal.log("[Shadowfront Import] Starting original-art and original-audio import.")
    results = [import_one(*item) for item in IMPORT_MANIFEST]
    unreal.EditorAssetLibrary.save_directory("/Game/Art", only_if_is_dirty=True, recursive=True)
    unreal.EditorAssetLibrary.save_directory("/Game/Audio", only_if_is_dirty=True, recursive=True)

    complete_count = sum(results)
    if complete_count != len(results):
        unreal.log_error(f"[Shadowfront Import] Incomplete import: {complete_count}/{len(results)} assets ready.")
        raise RuntimeError("Shadowfront asset import did not complete")

    unreal.log(f"[Shadowfront Import] Complete: {complete_count}/{len(results)} original assets available in /Game.")


if __name__ == "__main__":
    main()
