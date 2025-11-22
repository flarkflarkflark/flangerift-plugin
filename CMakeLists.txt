cmake_minimum_required(VERSION 3.22)

project(FLANGERIFT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

if(DEFINED JUCE_DIR)
    add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)
else()
    message(FATAL_ERROR "JUCE_DIR not specified")
endif()

juce_add_plugin(FLANGERIFT
    COMPANY_NAME "CustomAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Caud
    PLUGIN_CODE Flar
    FORMATS VST3 CLAP
    PRODUCT_NAME "FLANGERIFT")

target_sources(FLANGERIFT
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp)

target_compile_definitions(FLANGERIFT
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(FLANGERIFT
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
