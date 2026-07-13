cmake_minimum_required(VERSION 3.20)
project(GodModeToggle LANGUAGES CXX)

add_library(GodModeToggle SHARED src/main.cpp)
target_compile_features(GodModeToggle PRIVATE cxx_std_17)
target_compile_definitions(GodModeToggle PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(GodModeToggle PRIVATE "${CMAKE_SOURCE_DIR}/ScriptHookV.lib")

set_target_properties(GodModeToggle PROPERTIES
    PREFIX ""
    SUFFIX ".asi"
    OUTPUT_NAME "GodModeToggle"
)
