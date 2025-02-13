#include "TangoSolverMenu.h"

#include <iostream>

#include "imgui.h"
#include "../UI/ImGuiExtensions.h"

void TangoSolverMenu::onAttach(Window* window)
{
    lastValue = TangoSolver::Type::NONE;
    setGridSize(10);
}

void TangoSolverMenu::render(Window* window)
{
    if(!open)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("QueenSolver", &open, ImGuiWindowFlags_NoResize);

    if (ImGui::SliderInt("Size", &gridSize, 0, MAX_GRID_SIZE))
    {
        setGridSize(gridSize);
    }

    ImGui::SameLine();

    if(ImGui::Button("SOLVE"))
    {
        solve();
    }

    if(ImGui::Extension::ColoredButton("None", getColorForValue(TangoSolver::Type::NONE)))
    {
        lastValue = TangoSolver::Type::NONE;
    }
    ImGui::SameLine();

    if (ImGui::Extension::ColoredButton("Sun", getColorForValue(TangoSolver::Type::SUN)))
    {
        lastValue = TangoSolver::Type::SUN;
    }

    ImGui::SameLine();
    if (ImGui::Extension::ColoredButton("Moon", getColorForValue(TangoSolver::Type::MOON)))
    {
        lastValue = TangoSolver::Type::MOON;
    }

    if(ImGui::Button("<-- From Clipboard"))
    {
        fromClipboard();
    }

    ImGui::SameLine();

    if(ImGui::Button("--> To Clipboard"))
    {
        toClipboard();
    }

    if (gridSize > 0 && ImGui::BeginTable("TangoGrid", gridSize, ImGuiTableColumnFlags_NoResize))
    {
        for (int i = 0; i < gridSize; i++)
        {
            ImGui::TableSetupColumn(std::to_string(i).data(), ImGuiTableColumnFlags_WidthFixed);
        }

        for (int y = 0; y < gridSize; y++)
        {
            ImGui::PushID(y);
            for (int x = 0; x < gridSize; x++)
            {
                ImGui::PushID(x);
                ImGui::TableNextColumn();
                ImVec4 color = result != nullptr
                    ? getColorForValue((*result)[y][x])
                    : getColorForValue(gridValues[y][x]);

                std::string text = std::to_string(gridValues[y][x]);

                if(ImGui::Extension::ColoredButton("", color, {32, 32}))
                {
                    switch(gridValues[y][x])
                    {
                    case TangoSolver::Type::NONE:
                        gridValues[y][x] = TangoSolver::Type::SUN;
                        break;

                    case TangoSolver::Type::SUN:
                        gridValues[y][x] = TangoSolver::Type::MOON;
                        break;

                    case TangoSolver::Type::MOON:
                        gridValues[y][x] = TangoSolver::Type::NONE;
                        break;

                    default:
                        break;
                    }

                    lastValue = gridValues[y][x];
                }

                if (ImGui::GetIO().MouseDown[2] && ImGui::IsItemHovered())
                {
                    gridValues[y][x] = lastValue;
                }

                ImGui::PopID();
            }
            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void TangoSolverMenu::setGridSize(int size)
{
    gridSize = size;
    gridValues.clear();

    gridValues.reserve(gridSize);
    for (int y = 0; y < gridSize; y++)
    {
        std::vector<uint8_t> vector {};
        vector.reserve(gridSize);
        for (int x = 0; x < gridSize; x++)
        {
            vector.push_back(TangoSolver::Type::NONE);
        }
        gridValues.push_back(vector);
    }
}

void TangoSolverMenu::solve()
{
    TangoSolver solver {};
    solver.setGrid(gridValues);
    auto start = std::chrono::high_resolution_clock::now();
    result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Tango-Solve-Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
}

ImVec4 TangoSolverMenu::getColorForValue(uint8_t color)
{
    switch(color)
    {
    case TangoSolver::Type::SUN:
        return { 255.0 / 255, 179.0 / 255, 30.0 / 255, 1.0 };
    case TangoSolver::Type::MOON:
        return { 76.0 / 255, 140.0 / 255, 230.0 / 255, 1.0 };
    case TangoSolver::Type::NONE:
        return { 50.0 / 255,  50.0 / 255,  50.0 / 255, 1.0 };
    default:
        return { -1, -1, -1, -1 };
    }
}

void TangoSolverMenu::toClipboard() const
{
    std::string result;

    for(int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            result.push_back(gridValues[y][x]);
        }
    }

    ImGui::SetClipboardText(result.data());
}

void TangoSolverMenu::fromClipboard()
{
    std::string data = ImGui::GetClipboardText();
    fromText(data);
}

void TangoSolverMenu::fromText(const std::string& text)
{
    std::string num;

    gridSize = static_cast<int>(std::sqrt(text.size()));

    gridValues.clear();
    gridValues.reserve(gridSize);
    int offset = 0;
    for (int x = 0; x < gridSize; x++)
    {
        std::vector<uint8_t> vector{};
        vector.reserve(gridSize);

        for (int y = 0; y < gridSize; y++)
        {
            vector.push_back(text[offset]);
            offset++;
        }
        gridValues.push_back(vector);
    }
}
