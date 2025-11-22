# CLAUDE.md - AI Assistant Guide for FLANGERIFT Plugin

## Project Overview

**FLANGERIFT** is a JUCE-based audio plugin project that provides audio processing capabilities in VST3 and CLAP formats.

- **Project Name**: FLANGERIFT
- **Version**: 1.0.0
- **Company**: flarkAUDIO
- **Plugin Formats**: VST3, CLAP
- **Plugin Type**: Audio Effect (not a synthesizer)
- **Framework**: JUCE 7.0.9
- **Language**: C++17
- **Build System**: CMake 3.22+

## Repository Structure

```
flangerift-plugin/
├── .github/
│   └── workflows/
│       └── build.yml          # CI/CD configuration for Linux and Windows builds
├── src/
│   ├── PluginProcessor.h      # Audio processor header (FlangeriftAudioProcessor)
│   ├── PluginProcessor.cpp    # Audio processor implementation
│   ├── PluginEditor.h         # GUI editor header (FlangeriftAudioProcessorEditor)
│   └── PluginEditor.cpp       # GUI editor implementation
├── CMakeLists.txt             # CMake build configuration
└── README.md                  # Project documentation
```

## Architecture

### Core Components

#### 1. FlangeriftAudioProcessor (`src/PluginProcessor.h/cpp`)
- Main audio processing class inheriting from `juce::AudioProcessor`
- Handles audio buffer processing in the `processBlock()` method
- Currently implements simple pass-through (line 127 in PluginProcessor.cpp)
- Supports stereo input/output configuration
- No MIDI input/output functionality
- No tail length (immediate processing)

**Key Methods:**
- `prepareToPlay()`: Initialize DSP processing (line 85)
- `processBlock()`: Main audio processing loop (line 115)
- `releaseResources()`: Cleanup on stop (line 90)
- `getStateInformation()`/`setStateInformation()`: State persistence (lines 140-147)
- `createEditor()`: Creates GUI editor instance (line 135)

#### 2. FlangeriftAudioProcessorEditor (`src/PluginEditor.h/cpp`)
- GUI component inheriting from `juce::AudioProcessorEditor`
- Default size: 400x300 pixels (line 7 in PluginEditor.cpp)
- Simple visual design with black background and orange "FLANGERIFT" text
- Uses custom color: `0xfff97316` (orange, line 18 in PluginEditor.cpp)

**Key Methods:**
- `paint()`: Renders the UI (line 14)
- `resized()`: Handle layout changes (line 23)

### Plugin Configuration

From `CMakeLists.txt`:
- **Manufacturer Code**: `Caud`
- **Plugin Code**: `Flar`
- **Company Name**: `flarkAUDIO`
- **Product Name**: `FLANGERIFT`
- **Copy Plugin After Build**: Enabled for easy testing

### Dependencies

**JUCE Modules** (lines 37-43 in CMakeLists.txt):
- `juce::juce_audio_utils` - Audio utilities
- `juce::juce_dsp` - DSP processing
- `juce::juce_recommended_config_flags` - JUCE configuration
- `juce::juce_recommended_warning_flags` - Compiler warnings

**Compile Definitions**:
- `JUCE_WEB_BROWSER=0` - Disables web browser
- `JUCE_USE_CURL=0` - Disables CURL
- `JUCE_VST3_CAN_REPLACE_VST2=0` - VST2 compatibility

## Build System

### CMake Configuration

**Key Requirements:**
- CMake 3.22 minimum
- JUCE_DIR must be specified (line 7-11 in CMakeLists.txt)
- C++17 standard

### Build Process

#### Local Build
```bash
# 1. Clone JUCE framework
git clone --depth 1 --branch 7.0.9 https://github.com/juce-framework/JUCE.git

# 2. Configure with CMake
cmake -B build -DJUCE_DIR=./JUCE -DCMAKE_BUILD_TYPE=Release

# 3. Build the plugin
cmake --build build --config Release
```

#### Platform-Specific Dependencies

**Linux** (lines 20-36 in build.yml):
- build-essential
- cmake, ninja-build
- libasound2-dev (ALSA)
- X11 libraries (libx11-dev, libxcb1-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev)
- libgl1-mesa-dev (OpenGL)
- libfreetype6-dev (fonts)
- libwebkit2gtk-4.1-dev (webkit)

**Windows**:
- Visual Studio 17 2022 with x64 architecture
- No additional dependencies required

### CI/CD Workflow

**Triggered by:**
- Pushes to `main` or `master` branches
- Pull requests to `main` or `master` branches
- Manual workflow dispatch

**Build Artifacts:**
- Linux: `FLANGERIFT-1.0.0-Linux-x64.tar.gz`
- Windows: `FLANGERIFT-1.0.0-Windows-x64.zip`

**Outputs:**
- VST3 bundles (directories on Linux, folders on Windows)
- CLAP files (single files)

## Development Conventions

### Code Style

1. **Naming Conventions:**
   - Classes: PascalCase with descriptive suffixes
     - `FlangeriftAudioProcessor` for main processor
     - `FlangeriftAudioProcessorEditor` for GUI editor
   - Methods: camelCase following JUCE conventions
   - Member variables: camelCase with descriptive names
   - References: Use `&` suffix (e.g., `processorRef` in line 15 of PluginEditor.h)

2. **JUCE Patterns:**
   - Always use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` macro for JUCE classes
   - Use `juce::ignoreUnused()` for unused parameters (lines 71-82 in PluginProcessor.cpp)
   - Prefix JUCE types with namespace (e.g., `juce::AudioProcessor`, `juce::Graphics`)
   - Use `explicit` constructors when taking single parameters (line 8 in PluginEditor.h)

3. **Audio Processing:**
   - Always use `juce::ScopedNoDenormals` in `processBlock()` (line 120 in PluginProcessor.cpp)
   - Clear unused output channels (lines 124-125 in PluginProcessor.cpp)
   - Handle channel layout properly in `isBusesLayoutSupported()` (lines 95-112)

4. **Memory Management:**
   - Use JUCE smart pointers where appropriate
   - Rely on RAII for resource management
   - Avoid raw pointers except for JUCE framework requirements

### File Organization

- **Headers**: Contain class declarations and inline methods only
- **Implementation**: Keep all method implementations in .cpp files
- **Includes**: Always include corresponding header first (e.g., line 1 in PluginProcessor.cpp)

### Git Workflow

**Recent Refactoring Pattern** (from git log):
- Class names have been standardized to use full JUCE-style names
- Recent commits show progression: `FlangeriftProcessor` → `FlangeriftAudioProcessor`
- Recent commits show progression: `FlangeriftEditor` → `FlangeriftAudioProcessorEditor`

**Commit Message Style:**
- Use descriptive imperative mood ("Update", "Refactor", "Add", "Fix")
- Be specific about what changed (e.g., "Refactor FlangeriftEditor to FlangeriftAudioProcessorEditor")

## Common Development Tasks

### Adding a Parameter

1. Add parameter declaration to `FlangeriftAudioProcessor` class
2. Initialize in constructor using `juce::AudioProcessorValueTreeState`
3. Use parameter in `processBlock()` method
4. Update `getStateInformation()`/`setStateInformation()` for persistence
5. Add UI controls in `FlangeriftAudioProcessorEditor`

### Implementing DSP Processing

1. Add necessary JUCE DSP modules to CMakeLists.txt if not already present
2. Declare DSP objects as member variables in `FlangeriftAudioProcessor`
3. Initialize in `prepareToPlay()` with sample rate and block size
4. Process audio in `processBlock()` (currently line 127 is marked as "Simple pass-through for now")
5. Clean up in `releaseResources()`

### Adding UI Components

1. Declare components as member variables in `FlangeriftAudioProcessorEditor`
2. Initialize in constructor (after line 7 in PluginEditor.cpp)
3. Make visible with `addAndMakeVisible()`
4. Layout in `resized()` method (line 23)
5. Handle painting in `paint()` if custom drawing needed

### Testing Changes

1. Build the plugin locally
2. Copy built VST3/CLAP to plugin directory
3. Test in DAW (Digital Audio Workstation)
4. Check for:
   - Audio processing correctness
   - UI responsiveness
   - State saving/loading
   - CPU usage

## Current State Analysis

### What's Implemented
- Basic JUCE plugin structure with processor and editor
- Audio pass-through functionality
- Simple GUI with branding
- Stereo input/output support
- State management hooks (though not fully implemented)
- Multi-platform build system (Linux and Windows)

### What's Not Implemented Yet
- Actual flanger effect processing (name suggests this is planned)
- Parameter controls and user interaction
- Preset management
- State persistence (hooks exist but empty)
- Advanced UI components (sliders, knobs, etc.)

### Known Patterns from Git History
- Recent focus on standardizing class names to JUCE conventions
- Migration to proper JUCE naming standards
- Company name updated to "flarkAUDIO"
- Build system recently enhanced with Windows support

## Important Notes for AI Assistants

### When Modifying Code

1. **Always read files before editing** - Never propose changes to code you haven't read
2. **Maintain JUCE conventions** - Follow established JUCE patterns and naming
3. **Preserve audio safety** - Never introduce clicks, pops, or audio artifacts
4. **Test builds** - Ensure changes compile before committing
5. **Update related files** - If changing processor, may need to update editor

### When Adding Features

1. **Start with processor** - Implement DSP logic in `FlangeriftAudioProcessor` first
2. **Then add UI** - Create controls in `FlangeriftAudioProcessorEditor` second
3. **Wire parameters** - Connect UI to DSP via parameter management
4. **Test thoroughly** - Audio plugins require careful testing

### Critical Safety Considerations

1. **Audio Thread Safety**: Never allocate memory in `processBlock()`
2. **Denormal Protection**: Always use `ScopedNoDenormals` in `processBlock()`
3. **Channel Validation**: Always validate input/output channel counts
4. **Buffer Boundaries**: Never read/write outside buffer bounds

### Build Requirements

- JUCE framework must be available and specified via `-DJUCE_DIR`
- Platform-specific dependencies must be installed
- Use JUCE 7.0.9 for compatibility (as specified in build.yml line 40)

### Version Information

- Current version: 1.0.0 (from CMakeLists.txt line 3)
- Update version in CMakeLists.txt for releases
- Version is used in artifact naming in CI/CD

## References

- JUCE Documentation: https://docs.juce.com/
- JUCE Forum: https://forum.juce.com/
- JUCE GitHub: https://github.com/juce-framework/JUCE
- VST3 SDK: https://github.com/steinbergmedia/vst3sdk
- CLAP: https://github.com/free-audio/clap

## Questions to Ask Users

When working on this codebase, consider asking:

1. **For new features**: "What type of flanger algorithm do you want to implement?" (classic, through-zero, etc.)
2. **For parameters**: "What parameter ranges and default values do you prefer?"
3. **For UI changes**: "Do you have specific design preferences or color schemes?"
4. **For builds**: "Which DAW(s) do you plan to test with?"
5. **For releases**: "Should we update the version number for this change?"

## File Location Quick Reference

- Main processor class: `src/PluginProcessor.h` and `src/PluginProcessor.cpp`
- GUI editor class: `src/PluginEditor.h` and `src/PluginEditor.cpp`
- Build configuration: `CMakeLists.txt`
- CI/CD pipeline: `.github/workflows/build.yml`
- Plugin branding text: Line 20 in `src/PluginEditor.cpp`
- Company name: Line 14 in `CMakeLists.txt`
- Version number: Line 3 in `CMakeLists.txt`
