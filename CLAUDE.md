# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OBSInfoWriter is an OBS Studio plugin (C++17) that writes timestamps and metadata to files during streaming/recording sessions. It supports multiple output formats (plain text, CSV, EDL, SRT) and provides 14 customizable hotkeys for event marking.

## Build Commands

The project uses CMake (3.28+) with presets. Crosscables dependency is auto-fetched via FetchContent.

```bash
# Linux
cmake --preset ubuntu-x86_64
cmake --build --preset ubuntu-x86_64

# macOS (universal binary)
cmake --preset macos
cmake --build --preset macos

# Windows
cmake --preset windows-x64
cmake --build --preset windows-ci-x64
```

There is no test suite. A standalone test console exists in `main.cpp`/`DummyUtils.cpp` but is commented out in CMakeLists.txt.

## Architecture

**Plugin entry point**: `OBSStudioInfoWriter.cpp` — Registers the plugin with OBS, defines 14 hotkey handlers, settings UI properties, and hooks into OBS frontend events (recording/streaming start/stop, scene changes).

**Core class**: `InfoWriter` — Orchestrates all writing. Manages recording/streaming state, pause/resume, scene changes, and delegates to an output format. Uses timers from Crosscables' Groundfloor library for elapsed time tracking.

**Settings**: `InfoWriterSettings` — Stores configuration (hotkey texts, output format selection, file naming with strftime variables, logging flags). Serialized via OBS data API.

**Output formats** (`OutputFormat/` directory): All implement the `IOutputFormat` interface defined in `OutputFormat.h`. Implementations:
- `OutputFormatDefault` — simple timestamped text lines
- `OutputFormatCSV` — comma-separated values
- `OutputFormatEDL` — Edit Decision List (video editing)
- `OutputFormatSRT` — SubRip subtitle format

**Utilities**: `InfoWriterObsUtils` — helper to sync log filename with the video recording path.

## Code Style

- Uses `.clang-format` (clang-format 16+): tabs for indentation, 8-space width, 120-column limit, C++17
- Braces on same line except after function definitions
- Pointer alignment: right (`char *ptr`)
- CI runs format checks via `.github/workflows/check-format.yaml`

## Key Dependencies

- **libobs** + **obs-frontend-api**: OBS Studio plugin SDK
- **Crosscables/Groundfloor**: File I/O, string handling, timestamps (fetched automatically from GitHub)

## Release Process

The plugin version is stored in **two** files and both must be updated when cutting a release. Bump them manually as a normal commit before triggering the release workflow — do not rely on the workflow's auto-bump.

1. `buildspec.json` — the `"version"` field (e.g. `"version": "2.8.0"`). Accepts full version strings including prerelease suffixes like `2.8.0-beta1`.
2. `CMakeLists.txt` — the `project()` line (e.g. `project(OBSInfoWriter VERSION 2.8.0)`). Only accepts `X.Y.Z`, no suffix.

After the bump is committed and pushed to `master`, trigger the GitHub Actions Release workflow to tag, build, and create the draft release:

```bash
gh workflow run release.yaml -f version=2.8.0 --ref master
```

The workflow validates the format as `^[0-9]+\.[0-9]+\.[0-9]+(-(beta|rc)[0-9]*)?$`, creates the tag (bare numeric, e.g. `2.8.0` — no `v` prefix), and produces a draft GitHub release with build artifacts and checksums.

No CHANGELOG, README, or locale strings need updating — only the two version files above.
