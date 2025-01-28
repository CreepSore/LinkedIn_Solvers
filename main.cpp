#include "main.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "Menu.h"
#include "Window.h"

/*
const uint8_t p = 0;
const uint8_t o = 1;
const uint8_t b = 2;
const uint8_t g = 3;
const uint8_t w = 4;
const uint8_t r = 5;
const uint8_t y = 6;

auto solver = Solver();
solver.setGrid({
    {p, p, o, o, o, b, g},
    {p, p, o, o, o, g, g},
    {p, p, p, w, g, g, g},
    {p, w, w, w, w, w, g},
    {p, p, r, r, r, g, g},
    {p, p, r, y, r, g, g},
    {p, p, r, y, r, g, g}
});

auto start = std::chrono::high_resolution_clock::now();
auto solved = solver.solve();
auto end = std::chrono::high_resolution_clock::now();

std::cout << "Runtime: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start) << "\n\n";

if(solved == nullptr)
{
    std::cout << "No solution found!\n";
    return 1;
}

std::set<uint32_t> positions;

for (auto position: *solved)
{
    positions.insert(position.hash);
}

auto grid = solver.getGrid();
size_t size = grid.size();
for(size_t y = 0; y < size; y++)
{
    for(size_t x = 0; x < size; x++)
    {
        if(positions.contains(Vec2(x, y).hash))
        {
            std::cout << "Q";
            continue;
        }

        std::cout << ".";
    }
    std::cout << "\n";
}

for (auto vec2 : *solved)
{
    std::cout << vec2.x << "x" << vec2.y << "y\n";
}

return 0;
*/

void render()
{
    /*
    static bool queenSolverOpen = false;

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("MENU", 0, ImGuiWindowFlags_NoResize);

    if(ImGui::Button("QueenSolver"))
    {
        gridSize = 10;
        queenSolverOpen = true;
    }

    ImGui::End();

    if(queenSolverOpen)
    {
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
        ImGui::Begin("QueenSolver", 0, ImGuiWindowFlags_NoResize);

        if(ImGui::SliderInt4("Size", &gridSize, 0, 10))
        {
            gridValues.clear();

            for(int i = 0; i < gridSize*gridSize; i++)
            {
                gridValues.emplace_back(-1);
            }

            gridValues.resize(gridSize * gridSize);
        }

        if(gridSize > 0 && ImGui::BeginTable("QueenGrid", gridSize, ImGuiTableColumnFlags_NoResize))
        {
            for(int i = 0; i < gridSize; i++)
            {
                ImGui::TableSetupColumn(std::to_string(i).data(), ImGuiTableColumnFlags_WidthFixed);
            }

            for(int y = 0; y < gridSize; y++)
            {
                ImGui::PushID(y);
                for(int x = 0; x < gridSize; x++)
                {
                    ImGui::PushID(x);
                    ImGui::TableNextColumn();
                    ImGui::PopID();
                }
                ImGui::PopID();
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }
    */
}

int init()
{
    Menu menu{};
    Window window;
    window.show();

    window.addRenderable(&menu);

    while(true)
    {
        if(!window.tick())
        {
            break;
        }
    }

    return 0;
}

int main()
{
    return init();
}
