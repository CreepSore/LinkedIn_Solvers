cmake_minimum_required (VERSION 3.8)

option(BUILD_SHARED_LIBS OFF)

include(FetchContent)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Queen-Solver")

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

add_executable (Queen-Solver "main.cpp" "main.h" "Solver.h" "Solver.cpp" "IRenderable.h" "Window.h" "Window.cpp" "Menu.h" "Menu.cpp" "QueenSolverMenu.cpp" "QueenSolverMenu.h")

target_include_directories(
    Queen-Solver
    PRIVATE
    ${IMGUI_DIR}
)

target_link_libraries(
    Queen-Solver
    PRIVATE
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
)

set_property(TARGET Queen-Solver PROPERTY CXX_STANDARD 20)
