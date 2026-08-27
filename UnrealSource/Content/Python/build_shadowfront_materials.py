"""Create lightweight mobile materials from imported Shadowfront textures.

Run after import_shadowfront_assets.py with UnrealEditor-Cmd. Materials use a
single Base Color texture and a constant roughness to keep the mobile shader
cost predictable for the Pier Seven combat slice.
"""

import unreal


MATERIALS = (
    ("M_SF_Concrete", "/Game/Art/Textures/shadowfront_wet_concrete_material.shadowfront_wet_concrete_material", 0.78),
    ("M_SF_RustedSteel", "/Game/Art/Textures/shadowfront_rusted_steel_material.shadowfront_rusted_steel_material", 0.66),
)
MATERIAL_DIR = "/Game/Art/Materials"


def create_material(material_name: str, texture_path: str, roughness: float) -> bool:
    material_path = f"{MATERIAL_DIR}/{material_name}.{material_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(material_path):
        unreal.log(f"[Shadowfront Materials] Keeping existing material: {material_path}")
        return True

    texture = unreal.load_asset(texture_path)
    if not texture:
        unreal.log_error(f"[Shadowfront Materials] Missing texture: {texture_path}")
        return False

    factory = unreal.MaterialFactoryNew()
    material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        material_name, MATERIAL_DIR, unreal.Material, factory
    )
    if not material:
        unreal.log_error(f"[Shadowfront Materials] Could not create material: {material_name}")
        return False

    texture_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, -360, 0
    )
    texture_sample.set_editor_property("texture", texture)
    roughness_expression = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant, -360, 175
    )
    roughness_expression.set_editor_property("r", roughness)
    unreal.MaterialEditingLibrary.connect_material_property(
        texture_sample, "RGB", unreal.MaterialProperty.MP_BASE_COLOR
    )
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness_expression, "", unreal.MaterialProperty.MP_ROUGHNESS
    )
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_asset(material_path, only_if_is_dirty=False)
    unreal.log(f"[Shadowfront Materials] Created {material_path}")
    return True


def main() -> None:
    created = [create_material(*definition) for definition in MATERIALS]
    if not all(created):
        raise RuntimeError("Shadowfront material generation did not complete")
    unreal.EditorAssetLibrary.save_directory(MATERIAL_DIR, only_if_is_dirty=True, recursive=True)
    unreal.log("[Shadowfront Materials] Original environment materials are ready.")


if __name__ == "__main__":
    main()
