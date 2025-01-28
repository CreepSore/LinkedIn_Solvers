#include "QueenSolverMenu.h"

#include <iostream>

#include "imgui.h"

void QueenSolverMenu::onAttach(Window* window)
{
    setGridSize(10);
}

void QueenSolverMenu::render(Window* window)
{
    if(!open)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("QueenSolver", 0, ImGuiWindowFlags_NoResize);

    if (ImGui::SliderInt("Size", &gridSize, 0, MAX_GRID_SIZE))
    {
        setGridSize(gridSize);
    }

    ImGui::SameLine();

    if(ImGui::Button("SOLVE"))
    {
        solve();
    }

    for(int i = 1; ; i++) {
        ImVec4 colorButtonColor = getColorForNumber(i);

        if(colorButtonColor.x == -1)
        {
            break;
        }

        ImGui::PushStyleColor(ImGuiCol_Button, colorButtonColor);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));

        if(ImGui::Button(std::to_string(i).data()))
        {
            lastColor = i;
        }

        ImGui::PopStyleColor(2);
        ImGui::SameLine();
    }

    ImGui::NewLine();

    if(ImGui::Button("<-- From Clipboard"))
    {
        fromClipboard();
    }

    ImGui::SameLine();

    if(ImGui::Button("--> To Clipboard"))
    {
        toClipboard();
    }

    if (gridSize > 0 && ImGui::BeginTable("QueenGrid", gridSize, ImGuiTableColumnFlags_NoResize))
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
                ImVec4 color = getColorForNumber(gridValues[y][x]);

                std::string text = std::to_string(gridValues[y][x]);

                if(result != nullptr)
                {
                    for (auto value : *result)
                    {
                        if(value.x == x && value.y == y)
                        {
                            color = ImVec4(0, 1, 0, 1);
                            text = "Q";
                            break;
                        }
                    }
                }

                if (color.x != -1)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, color);
                }
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));

                if(ImGui::Button(text.data(), {32, 32}))
                {
                    gridValues[y][x] += 1;
                    lastColor = gridValues[y][x];
                }

                if (ImGui::GetIO().MouseClicked[1] && ImGui::IsItemHovered())
                {
                    if (gridValues[y][x] > 0)
                    {
                        gridValues[y][x] -= 1;
                    }
                    lastColor = gridValues[y][x];
                }

                if (ImGui::GetIO().MouseDown[2] && ImGui::IsItemHovered())
                {
                    gridValues[y][x] = lastColor;
                }

                if(color.x != -1)
                {
                    ImGui::PopStyleColor(1);
                }

                ImGui::PopStyleColor(1);

                ImGui::PopID();
            }
            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void QueenSolverMenu::setGridSize(int size)
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
            vector.push_back(0);
        }
        gridValues.push_back(vector);
    }
}

void QueenSolverMenu::solve()
{
    Solver solver {};
    solver.setGrid(gridValues);
    auto start = std::chrono::high_resolution_clock::now();
    result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Queen-Solve-Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
}

ImVec4 QueenSolverMenu::getColorForNumber(uint8_t color)
{
    switch(color)
    {
    case 1:
        return { 187.0 / 255, 163.0 / 255, 225.0 / 255, 1.0 };
    case 2:
        return { 254.0 / 255, 202.0 / 255, 145.0 / 255, 1.0 };
    case 3:
        return { 150.0 / 255, 189.0 / 255, 254.0 / 255, 1.0 };
    case 4:
        return { 223.0 / 255, 223.0 / 255, 223.0 / 255, 1.0 };
    case 5:
        return { 254.0 / 255, 123.0 / 255, 95.0 / 255, 1.0 };
    case 6:
        return { 230.0 / 255, 243.0 / 255, 137.0 / 255, 1.0 };
    case 7:
        return { 163.0 / 255, 210.0 / 255, 216.0 / 255, 1.0 };
    case 8:
        return { 223.0 / 255, 160.0 / 255, 191.0 / 255, 1.0 };
    case 9:
        return { 185.0 / 255, 178.0 / 255, 159.0 / 255, 1.0 };
    case 10:
        return { 179.0 / 255, 223.0 / 255, 160.0 / 255, 1.0 };
    default:
        return { -1, -1, -1, -1 };
    }
}

void QueenSolverMenu::toClipboard() const
{
    std::string result;

    for(int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            if (y != 0 || x != 0) {
                result.push_back(',');
            }
            result.append(std::to_string(gridValues[y][x]));
        }
    }

    ImGui::SetClipboardText(result.data());
}

void QueenSolverMenu::fromClipboard()
{
    std::string data = ImGui::GetClipboardText();
    fromText(data);
}

void QueenSolverMenu::fromText(const std::string& text)
{
    std::string num;
    std::vector<uint8_t> dataVector;

    for (const char c : text)
    {
        if (c == ',')
        {
            dataVector.push_back(atoi(num.data()));
            num.clear();
            continue;
        }

        num.push_back(c);
    }

    if (!num.empty())
    {
        dataVector.push_back(atoi(num.data()));
    }

    gridSize = static_cast<int>(std::sqrt(dataVector.size()));

    gridValues.clear();
    gridValues.reserve(gridSize);
    int offset = 0;
    for (int y = 0; y < gridSize; y++)
    {
        std::vector<uint8_t> vector{};
        vector.reserve(gridSize);

        for (int x = 0; x < gridSize; x++)
        {
            vector.push_back(dataVector[offset]);
            offset++;
        }
        gridValues.push_back(vector);
    }
}
