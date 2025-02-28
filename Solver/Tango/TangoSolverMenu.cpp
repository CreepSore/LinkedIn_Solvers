#include "TangoSolverMenu.h"

#include <iostream>

#include "imgui.h"
#include "../UI/ImGuiExtensions.h"

void TangoSolverMenu::onAttach(Window* window)
{
    lastValue = TangoSolver::Type::FIELD_TYPE_NONE;
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

    if(ImGui::Extension::ColoredButton("None", getColorForValue(TangoSolver::Type::FIELD_TYPE_NONE)))
    {
        lastValue = TangoSolver::Type::FIELD_TYPE_NONE;
    }
    ImGui::SameLine();

    if (ImGui::Extension::ColoredButton("Sun", getColorForValue(TangoSolver::Type::FIELD_TYPE_SUN)))
    {
        lastValue = TangoSolver::Type::FIELD_TYPE_SUN;
    }

    ImGui::SameLine();
    if (ImGui::Extension::ColoredButton("Moon", getColorForValue(TangoSolver::Type::FIELD_TYPE_MOON)))
    {
        lastValue = TangoSolver::Type::FIELD_TYPE_MOON;
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

    if(result != nullptr)
    {
        grid->render({ .itemSpacing = ImVec2(0, 0), .renderAsChar = true });
    }
    else if(grid != nullptr)
    {
        grid->render({.itemSpacing = ImVec2(0, 0), .renderAsChar = true});
    }

    ImGui::End();
}

void TangoSolverMenu::setGridSize(int size)
{
    if(size == 0)
    {
        grid = nullptr;
        return;
    }

    gridSize = size;
    grid = std::make_unique<Grid<char>>((size * 2) - 1, (size * 2) - 1, TangoSolver::Type::FIELD_TYPE_NONE, [this](
        size_t x,
        size_t y,
        char value,
        GridMouseButton clickType,
        GridMouseEvent eventType
    )
    {
        gridMouseHandler(x, y, value, clickType, eventType);
    });

    initializeGrid();
}

void TangoSolverMenu::solve()
{
    TangoSolver solver {};
    solver.setGrid(grid.get());
    auto start = std::chrono::high_resolution_clock::now();
    result = solver.solve();

    for(size_t y = 0; y < result->getHeight(); y += 2)
    {
        for (size_t x = 0; x < result->getWidth(); x += 2)
        {
            grid->setColor(x, y, getColorForValue(result->getValue(x, y)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Tango-Solve-Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
}

ImVec4 TangoSolverMenu::getColorForValue(uint8_t color)
{
    switch(color)
    {
    case TangoSolver::Type::FIELD_TYPE_SUN:
        return { 255.0 / 255, 179.0 / 255, 30.0 / 255, 1.0 };
    case TangoSolver::Type::FIELD_TYPE_MOON:
        return { 76.0 / 255, 140.0 / 255, 230.0 / 255, 1.0 };
    case TangoSolver::Type::FIELD_TYPE_NONE:
        return { 50.0 / 255,  50.0 / 255,  50.0 / 255, 1.0 };
    default:
        return { -1, -1, -1, -1 };
    }
}

void TangoSolverMenu::toClipboard() const
{
    ImGui::SetClipboardText(grid->toString().data());
}

void TangoSolverMenu::fromClipboard()
{
    std::string data = ImGui::GetClipboardText();
    fromText(data);
}

void TangoSolverMenu::fromText(const std::string& text)
{
    grid = Grid<char>::constructFromString(text, [this](
        size_t x,
        size_t y,
        char value,
        GridMouseButton clickType,
        GridMouseEvent eventType
    )
    {
        gridMouseHandler(x, y, value, clickType, eventType);
    });
    grid->defaultValue = TangoSolver::Type::FIELD_TYPE_NONE;

    initializeGrid();
}

void TangoSolverMenu::initializeGrid()
{
    for (size_t y = 0; y < grid->getHeight(); y += 2)
    {
        for (size_t x = 0; x < grid->getWidth(); x += 2)
        {
            grid->setColor(x, y, getColorForValue(grid->getValue(x, y)));
        }
    }

    for (size_t x = 1; x < grid->getWidth(); x += 2)
    {
        for (size_t y = 0; y < grid->getHeight(); y++)
        {
            grid->buttonSize.insert_or_assign(Vec2(x, y), ImVec2(16, 32));
            grid->setColor(x, y, ImVec4(0.1, 0.1, 0.1, 1));
        }
    }

    for (size_t y = 1; y < grid->getHeight(); y += 2)
    {
        for (size_t x = 0; x < grid->getWidth(); x++)
        {
            grid->buttonSize.insert_or_assign(Vec2(x, y), ImVec2(32, 16));
            grid->setColor(x, y, ImVec4(0.1, 0.1, 0.1, 1));
        }
    }

    for (size_t y = 1; y < grid->getHeight(); y += 2)
    {
        for (size_t x = 1; x < grid->getWidth(); x += 2)
        {
            grid->setColor(x, y, ImVec4(0, 0, 0, 0));
            grid->buttonSize.insert_or_assign(Vec2(x, y), ImVec2(16, 16));
        }
    }
}

void TangoSolverMenu::gridMouseHandler(
    size_t x,
    size_t y,
    char value,
    GridMouseButton clickType,
    GridMouseEvent eventType
)
{
    if (eventType != GridMouseEvent::CLICK)
    {
        return;
    }

    if (x % 2 == 0 && y % 2 == 0)
    {
        if (value == TangoSolver::Type::FIELD_TYPE_NONE)
        {
            grid->setValue(x, y, TangoSolver::Type::FIELD_TYPE_SUN);
        }
        else if (value == TangoSolver::Type::FIELD_TYPE_SUN)
        {
            grid->setValue(x, y, TangoSolver::Type::FIELD_TYPE_MOON);
        }
        else if (value == TangoSolver::Type::FIELD_TYPE_MOON)
        {
            grid->setValue(x, y, TangoSolver::Type::FIELD_TYPE_NONE);
        }

        grid->setColor(x, y, getColorForValue(grid->getValue(x, y)));

        return;
    }

    if (x % 2 == 0 || y % 2 == 0)
    {
        if (value == ' ')
        {
            grid->setValue(x, y, 'X');
        }
        else if (value == 'X')
        {
            grid->setValue(x, y, '=');
        }
        else if (value == '=')
        {
            grid->setValue(x, y, ' ');
        }
    }
}
