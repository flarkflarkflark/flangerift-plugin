cmake_minimum_required(VERSION 3.22)

project(FLANGERIFT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable LTO to avoid linker issues
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

# Find JUCE
if(DEFINED JUCE_DIR)
    add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)
else()
    message(FATAL_ERROR "JUCE_DIR not specified")
endif()

# Plugin formats
juce_add_plugin(FLANGERIFT
    COMPANY_NAME "CustomAudio"
    BUNDLE_ID "com.customaudio.flangerift"
    
    PLUGIN_MANUFACTURER_CODE Caud
    PLUGIN_CODE Flar
    
    FORMATS VST3 CLAP
    
    PRODUCT_NAME "FLANGERIFT"
    
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    
    COPY_PLUGIN_AFTER_BUILD FALSE
    
    VST3_CATEGORIES Fx
    CLAP_ID "com.customaudio.flangerift"
)

# Add all source files
target_sources(FLANGERIFT
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
)

# Include directories
target_include_directories(FLANGERIFT
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Compile definitions
target_compile_definitions(FLANGERIFT
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_STRICT_REFCOUNTEDPOINTER=1
)

# Link JUCE modules
target_link_libraries(FLANGERIFT
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

# Don't use LTO flags that cause linker issues
# Remove the juce_recommended_lto_flags

# Platform-specific settings
if(APPLE)
    set_target_properties(FLANGERIFT PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(FLANGERIFT PRIVATE dl pthread)
endif()
