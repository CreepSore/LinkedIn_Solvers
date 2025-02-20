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
    ImGui::Begin("QueenSolver", &open, ImGuiWindowFlags_NoResize);

    int gridSize = grid->getWidth();
    if (ImGui::SliderInt("Size", &gridSize, 0, MAX_GRID_SIZE))
    {
        setGridSize(gridSize);
    }

    ImGui::SameLine();

    if(ImGui::Button("SOLVE"))
    {
        solve();
    }

    for(size_t i = 1; ; i++) {
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

    grid->render(ImVec2(32, 32), ImVec2(1, 1));

    ImGui::End();
}

void QueenSolverMenu::setGridSize(int size)
{
    grid = std::make_unique<Grid<>>(
        size, 
        size,
        0
    );

    attachMouseHandlerToGrid();
}

void QueenSolverMenu::solve()
{
    QueenSolver solver {};
    solver.setGrid(*grid->getResult());

    auto start = std::chrono::high_resolution_clock::now();
    result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Queen-Solve-Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";

    for (const auto& value : *result)
    {
        grid->setColor(value.x, value.y, ImVec4(0, 1, 0, 1));
    }
}

ImVec4 QueenSolverMenu::getColorForNumber(size_t color)
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
    ImGui::SetClipboardText(grid->toString().data());
}

void QueenSolverMenu::fromClipboard()
{
    std::string data = ImGui::GetClipboardText();
    fromText(data);
}

void QueenSolverMenu::fromText(const std::string& text)
{
    grid = Grid<>::fromString(text);

    for(size_t x = 0; x < grid->getWidth(); x++)
    {
        for(size_t y = 0; y < grid->getHeight(); y++)
        {
            grid->setColor(x, y, getColorForNumber(grid->getValue(x, y)));
        }
    }

    attachMouseHandlerToGrid();
}

void QueenSolverMenu::attachMouseHandlerToGrid()
{
    grid->setMouseEventHandler([this](
        auto x,
        auto y,
        auto value,
        auto mouseType,
        auto eventType
    ) {
        if (eventType == GridMouseEvent::DOWN)
        {
            if (mouseType == GridMouseButton::MIDDLE)
            {
                grid->setValue(x, y, lastColor);
                grid->setColor(x, y, getColorForNumber(lastColor));
            }
        }
        else if (eventType == GridMouseEvent::CLICK)
        {
            if (mouseType == GridMouseButton::LEFT)
            {
                auto& valRef = grid->getValue(x, y);
                valRef += 1;
                lastColor = valRef;
                grid->setColor(x, y, getColorForNumber(valRef));
            }
            else if (mouseType == GridMouseButton::RIGHT)
            {
                auto& valRef = grid->getValue(x, y);
                if (valRef > 0)
                {
                    valRef -= 1;
                }
                lastColor = valRef;
                grid->setColor(x, y, getColorForNumber(valRef));
            }
        }
    });
}
