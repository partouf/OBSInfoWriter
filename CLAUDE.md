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
