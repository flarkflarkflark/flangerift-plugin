cmake_minimum_required(VERSION 3.22)

project(FLANGERIFT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE
if(DEFINED JUCE_DIR)
    add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)
else()
    message(FATAL_ERROR "JUCE_DIR not specified")
endif()

# Define formats
set(FORMATS VST3 CLAP)

juce_add_plugin(FLANGERIFT
    COMPANY_NAME "CustomAudio"
    BUNDLE_ID "com.customaudio.flangerift"
    PLUGIN_MANUFACTURER_CODE Caud
    PLUGIN_CODE Flar
    FORMATS ${FORMATS}
    PRODUCT_NAME "FLANGERIFT"
    
    # Plugin characteristics
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    
    # VST3 settings
    VST3_CATEGORIES Fx
    
    # CLAP settings
    CLAP_ID "com.customaudio.flangerift"
    CLAP_FEATURES "audio-effect" "stereo"
)

# Generate JUCE header
juce_generate_juce_header(FLANGERIFT)

# Add source files
target_sources(FLANGERIFT
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
        src/PluginEditor.cpp
        src/PluginEditor.h
)

# Set include directories
target_include_directories(FLANGERIFT
    PRIVATE
        src
)

# Compile definitions
target_compile_definitions(FLANGERIFT
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_STRICT_REFCOUNTEDPOINTER=1
        
        # Plugin metadata
        JucePlugin_Name="FLANGERIFT"
        JucePlugin_Desc="Combined flanger and filter with matrix interface"
        JucePlugin_Manufacturer="CustomAudio"
        JucePlugin_ManufacturerWebsite="https://github.com"
        JucePlugin_ManufacturerEmail="support@example.com"
        JucePlugin_ManufacturerCode=0x43617564
        JucePlugin_PluginCode=0x466c6172
        JucePlugin_IsSynth=0
        JucePlugin_WantsMidiInput=0
        JucePlugin_ProducesMidiOutput=0
        JucePlugin_IsMidiEffect=0
        JucePlugin_EditorRequiresKeyboardFocus=0
        JucePlugin_Version=1.0.0
        JucePlugin_VersionCode=0x10000
        JucePlugin_VersionString="1.0.0"
        JucePlugin_VSTUniqueID=JucePlugin_PluginCode
        JucePlugin_VSTCategory=kPlugCategEffect
        JucePlugin_Vst3Category="Fx"
        JucePlugin_AUMainType=0x61756678
        JucePlugin_CFBundleIdentifier=com.customaudio.flangerift
)

# Link libraries
target_link_libraries(FLANGERIFT
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Platform-specific settings
if(APPLE)
    set_target_properties(FLANGERIFT PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    )
endif()
