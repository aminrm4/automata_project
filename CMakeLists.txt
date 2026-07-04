cmake_minimum_required(VERSION 3.16)

project(automata_project
    VERSION 1.0
    DESCRIPTION "Universal Turing Machine Simulator with FTXUI"
    LANGUAGES CXX
)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v5.0.0
    GIT_SHALLOW    TRUE
)

set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(json ftxui)



file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "no cpp found")
endif()



add_executable(automata ${PROJECT_SOURCES})

target_include_directories(automata
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(automata
    PRIVATE
        nlohmann_json::nlohmann_json
        ftxui::screen
        ftxui::dom
        ftxui::component
)



set_target_properties(automata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "✅ automata_project configured successfully.")
message(STATUS "   Sources found: ${PROJECT_SOURCES}")
