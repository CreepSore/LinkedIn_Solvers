#include <cmath>
#include "QueensGenerator.h"
#include "../Shared/Vec2.h"

#include "imgui.h"

QueensGenerator::QueensGenerator()
{
    rndEngine = std::default_random_engine(rnd());

    for(int i = 0; i < 50; i++)
    {
        colors.emplace_back(getRandom(), getRandom(), getRandom(), 1.0);
    }
}

std::unique_ptr<Grid<uint8_t, int32_t>> QueensGenerator::generate(uint16_t size)
{
    if(size == 0)
    {
        return nullptr;
    }

    auto result = generateGrid(size);
    generateColors(*result);
    flipColorGrid(*result);

    return result;
}

double QueensGenerator::getRandom()
{
    return rndDistribution(rndEngine);
}

std::unique_ptr<Grid<uint8_t, int32_t>> QueensGenerator::generateGrid(uint16_t size)
{
    auto result = std::make_unique<Grid<uint8_t, int32_t>>(size, size, 0);
    blockedX.clear();
    blockedY.clear();
    placed.clear();

    blockedX.reserve(size);
    blockedY.reserve(size);
    placed.reserve(size);

    for (uint16_t i = 0; i < size; i++)
    {
        blockedX.push_back(false);
        blockedY.push_back(false);
    }

    bool direction = getRandom() > 0.5;
    double chancePerNode = 1.0 / size;
    bool retry = true;
    bool firstTry = true;

    while (retry)
    {
        if (!firstTry && retry)
        {
            placed.clear();
            result->reset();
            for (uint16_t i = 0; i < size; i++)
            {
                blockedX[i] = false;
                blockedY[i] = false;
            }
        }

        firstTry = false;
        retry = false;
        for (uint16_t i = 0; i < size; i++)
        {
            bool found = false;
            while (!found)
            {
                int validNodes = 0;
                for (uint16_t j = 0; j < size; j++)
                {
                    Vec2 pos = direction
                        ? Vec2(i, j)
                        : Vec2(j, i);

                    if (blockedX[pos.x] || blockedY[pos.y])
                    {
                        continue;
                    }

                    bool valid = true;
                    for (const Vec2& queen : placed)
                    {
                        if (std::abs(pos.x - queen.x) == 1 && std::abs(pos.y - queen.y) == 1)
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (!valid)
                    {
                        continue;
                    }

                    validNodes++;

                    if (getRandom() <= chancePerNode)
                    {
                        placed.push_back(pos);
                        result->setValue(pos.x, pos.y, 1);
                        found = true;
                        blockedX[pos.x] = true;
                        blockedY[pos.y] = true;
                    }
                }

                if (validNodes == 0)
                {
                    retry = true;
                    break;
                }
            }

        }
    }

    result->initializeColors(ImVec4(-1, -1, -1, -1));

    return result;
}

void QueensGenerator::generateColors(Grid<uint8_t, int32_t>& grid)
{
    size_t colorId = 0;
    for (const Vec2& pos : placed)
    {
        if(getRandom() > 0.75)
        {
            generateColorsRandomWalkLinear(grid, pos, colorId, grid.getWidth() * grid.getWidth());
        }
        else
        {
            generateColorsRandomWalk(grid, pos, colorId, grid.getWidth());
        }
        colorId++;
    }

    fillEmptyColors(grid);
}

void QueensGenerator::generateColorsRandomWalk(
    Grid<uint8_t, int32_t>& grid,
    const Vec2& position,
    size_t colorId,
    size_t step
)
{
    auto color = colors.size() > colorId
        ? colors[colorId]
        : ImVec4(-1, -1, -1, -1);

    grid.setColor(position.x, position.y, color);

    if(step - 1 == 0)
    {
        return;
    }

    std::vector offsets {
        Vec2(position.x, position.y + 1),
        Vec2(position.x + 1, position.y),
        Vec2(position.x, position.y - 1),
        Vec2(position.x - 1, position.y)
    };

    int totalWalked = 0;

    for (const Vec2& next : offsets)
    {
        if(next.x >= grid.rawGrid.size() || next.y >= grid.rawGrid.size())
        {
            continue;
        }

        if (grid.getColor(next.x, next.y).x != -1)
        {
            continue;
        }

        if(getRandom() > 0.4 + 0.1 * totalWalked)
        {
            totalWalked++;
            generateColorsRandomWalk(grid, next, colorId, step - 1);
        }
    }
}

void QueensGenerator::generateColorsRandomWalkLinear(
    Grid<uint8_t, int32_t>& grid,
    const Vec2& position,
    size_t colorId,
    size_t step,
    int direction,
    bool changedDirection
)
{
    int toWalk = direction;

    if(toWalk == -1)
    {
        if(getRandom() >= 0.75)
        {
            toWalk = 0;
        }
        else if (getRandom() >= 0.75)
        {
            toWalk = 1;
        }
        else if (getRandom() >= 0.75)
        {
            toWalk = 2;
        }
        else
        {
            toWalk = 3;
        }
    }

    auto color = colors.size() > colorId
        ? colors[colorId]
        : ImVec4(-1, -1, -1, -1);

    grid.setColor(position.x, position.y, color);

    if (step - 1 == 0)
    {
        return;
    }

    std::vector<Vec2> offsets;

    if(toWalk == 0)
    {
        offsets.emplace_back(position.x, position.y - 1);
    }
    else if (toWalk == 1)
    {
        offsets.emplace_back(position.x + 1, position.y);
    }
    else if (toWalk == 2)
    {
        offsets.emplace_back(position.x, position.y + 1);
    }
    else if (toWalk == 3)
    {
        offsets.emplace_back(position.x - 1, position.y);
    }

    for (const Vec2& next : offsets)
    {
        if (next.x >= grid.rawGrid.size() || next.y >= grid.rawGrid.size())
        {
            continue;
        }

        if (grid.getColor(next.x, next.y).x != -1 || grid.getValue(next.x, next.y) != 0)
        {
            continue;
        }

        if(getRandom() > (changedDirection ? 0.95 : 0.3))
        {
            int newDirection = toWalk;

            newDirection += getRandom() > 0.5
                ? 1
                : -1;

            if(newDirection > 3)
            {
                newDirection = 0;
            }
            else if(newDirection < 0)
            {
                newDirection = 3;
            }

            generateColorsRandomWalkLinear(grid, next, colorId, step - 1, newDirection, true);
        }

        if(getRandom() > 0.5)
        {
            generateColorsRandomWalkLinear(grid, next, colorId, step - 1, toWalk, changedDirection);
        }
    }
}

void QueensGenerator::fillEmptyColors(Grid<uint8_t, int32_t>& grid)
{
    for(int32_t y = 0; y < grid.getHeight(); y++)
    {
        for(int32_t x = 0; x < grid.getWidth(); x++)
        {
            ImVec4 color = grid.getColor(x, y);
            if(color.x != -1)
            {
                continue;
            }

            grid.setColor(x, y, getNearestColor(grid, x, y));
        }
    }
}

ImVec4 QueensGenerator::getNearestColor(const Grid<uint8_t, int32_t>& grid, int32_t x, int32_t y)
{
    double maxDistance = DBL_MAX;
    ImVec4 result;

    for (int32_t cY = 0; cY < grid.getHeight(); cY++)
    {
        for (int32_t cX = 0; cX < grid.getWidth(); cX++)
        {
            ImVec4 color = grid.getColor(cX, cY);
            if (color.x == -1)
            {
                continue;
            }

            double dX = cX - x;
            double dY = cY - y;
            double distance = std::abs(std::sqrt(dX*dX + dY*dY));

            if(distance < maxDistance)
            {
                result = color;
                maxDistance = distance;
            }
        }
    }

    return result;
}

void QueensGenerator::flipColorGrid(Grid<uint8_t, int32_t>& grid) const
{
    for (int32_t y = 0; y < grid.getHeight(); y++)
    {
        for (int32_t x = 0; x < grid.getWidth(); x++)
        {
            grid.setValue(x, y, colorToIndex(grid.colorGrid->getValue(x, y)) + 1);
        }
    }
}

size_t QueensGenerator::colorToIndex(ImVec4& color) const
{
    for(size_t i = 0; i < colors.size(); i++)
    {
        auto& clr = colors[i];
        if (clr.x == color.x && clr.y == color.y && clr.z == color.z)
        {
            return i;
        }
    }

    return -1;
}


QueensGeneratorMenu::QueensGeneratorMenu()
{

}

void QueensGeneratorMenu::render(Window* window)
{
    if (!open)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("QueensGenerator", &open, ImGuiWindowFlags_NoResize);

    ImGui::SliderInt("Size", &gridSize, 4, 50);

    if(ImGui::Button("Generate"))
    {
        grid = queensGenerator->generate(gridSize);
    }

    if(grid != nullptr)
    {
        grid->render(GridRenderOptions{ .buttonSize = ImVec2(32, 32), .itemSpacing = ImVec2(1, 1), .clipboardButtons = true, .autofit = true });
    }

    ImGui::End();

    IRenderable::render(window);
}

void QueensGeneratorMenu::onAttach(Window* window)
{
    queensGenerator = std::make_unique<QueensGenerator>();
    IRenderable::onAttach(window);
}
