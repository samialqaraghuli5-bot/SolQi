# Shadowfront

**Shadowfront: Ash Line** is an original single-player 3D mobile combat game produced by Sami. This repository stores the Unreal Engine project source and production documents; it intentionally excludes engine binaries, generated build products, external source media, Android signing material, and any server credentials.

## Current verified scope

The source currently contains a native C++ combat slice for Unreal Engine 5.8.2: a first-person player, hitscan weapon, reload and sprint behavior, health and regeneration, a runtime-built industrial arena, three enemy waves with four role archetypes, difficulty configuration, a HUD, and basic checkpoint-save data. Original English dialogue, music, narrative documents, visual identity, and acceptance criteria are maintained alongside the source.

This repository does **not** claim that an Android APK, a finished two-hour campaign, imported production assets, device-frame-rate measurements, or release signing is complete. The authoritative status is maintained in `PRODUCTION_ACCEPTANCE.md` and `BUILD_LOG.md`.

## Structure

| Path | Purpose |
| --- | --- |
| `UnrealSource/` | Unreal Engine project descriptor, config, C++ gameplay module, and repeatable asset-import utility. |
| `CAMPAIGN_SCOPE.md`, `CAMPAIGN_FLOW.md` | Original three-operation campaign design and pacing targets. |
| `DIALOGUE_BIBLE.md`, `AUDIO_DESIGN.md` | Original English dialogue and audio-direction plans. |
| `ASSET_REGISTRY.md`, `BRAND_IDENTITY.md` | Original asset inventory and Sami production identity. |
| `PRODUCTION_ACCEPTANCE.md`, `BUILD_LOG.md`, `todo.md` | Evidence-based progress, build record, and active implementation work. |

## Local build direction

The project is developed against Unreal Engine 5.8.2 source. A Linux game-target build is the first gate; Android cooking and packaging must follow only after the build is demonstrated successful and required editor/tools are present. No publishing or store-signing configuration is included in this repository.
