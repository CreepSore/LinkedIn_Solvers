cmake_minimum_required (VERSION 3.8)

option(BUILD_SHARED_LIBS OFF)

include(FetchContent)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("LinkedInSolver")

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.6
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable (LinkedInSolver "main.cpp" "main.h" "Queens/QueenSolver.h" "Queens/QueenSolver.cpp" "UI/IRenderable.h" "UI/Window.h" "UI/Window.cpp" "UI/Menu.h" "UI/Menu.cpp" "Queens/QueenSolverMenu.cpp" "Queens/QueenSolverMenu.h" "Tango/TangoSolver.cpp" "Tango/TangoSolver.h" "Shared/Vec2.h" "Tango/TangoSolverMenu.cpp" "Tango/TangoSolverMenu.h" "UI/ImGuiExtensions.h"  "Queens/QueensGenerator.h" "Queens/QueensGenerator.cpp" "UI/Grid.h")

target_include_directories(
    LinkedInSolver
    PRIVATE
    ${IMGUI_DIR}
)

target_link_libraries(
    LinkedInSolver
    PRIVATE
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
)

set_property(TARGET LinkedInSolver PROPERTY CXX_STANDARD 20)
