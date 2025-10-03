cmake_minimum_required(VERSION 3.15)
project(MidiApp)

set(CMAKE_CXX_STANDARD 20)

# Collect sources
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SOURCES
    ${SRC_DIR}/Main.cpp
    ${SRC_DIR}/RtMidi/RtMidi.cpp
)

# Include directories
include_directories(
    ${SRC_DIR}/MidiDevice
    ${SRC_DIR}/MidiMessage
    ${SRC_DIR}/RtMidi
)

# Platform-specific dependencies
if(WIN32)
    set(PLATFORM_LIBS winmm)
elseif(APPLE)
    find_library(COREMIDI CoreMIDI)
    find_library(COREFOUNDATION CoreFoundation)
    set(PLATFORM_LIBS ${COREMIDI} ${COREFOUNDATION})
elseif(UNIX)
    find_package(ALSA REQUIRED)
    set(PLATFORM_LIBS ${ALSA_LIBRARIES})
    include_directories(${ALSA_INCLUDE_DIRS})
endif()

# Build executable
add_executable(MidiApp ${SOURCES})
target_link_libraries(MidiApp PRIVATE ${PLATFORM_LIBS})


