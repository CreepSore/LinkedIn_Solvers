#include "QueenSolver.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>

QueenSolver::QueenSolver() = default;

void QueenSolver::setGrid(std::vector<std::vector<uint8_t>> grid)
{
    this->grid = grid;

    this->neededQueens = 0;
    for(size_t x = 0; x < this->grid.size(); x++)
    {
        for(size_t y = 0; y < this->grid[x].size(); y++)
        {
            this->colorFields[this->grid[y][x] - 1].emplace_back(x, y);

            if(this->grid[x][y] > this->neededQueens)
            {
                this->neededQueens = this->grid[x][y];
            }
        }
    }
}

std::vector<std::vector<uint8_t>>& QueenSolver::getGrid()
{
    return this->grid;
}

std::unique_ptr<std::vector<Vec2>> QueenSolver::solve()
{
    std::unique_ptr<std::vector<Vec2>> result(new std::vector<Vec2>());
    result->reserve(this->neededQueens);

    std::vector<bool>blockedX{};
    blockedX.reserve(this->grid.size());

    std::vector<bool> blockedY{};
    blockedY.reserve(this->grid.size());

    std::vector<bool> blockedColor{};
    blockedX.reserve(this->neededQueens);

    for(size_t i = 0; i < this->grid.size(); i++)
    {
        blockedX.push_back(false);
        blockedY.push_back(false);
    }

    for(size_t i = 0; i < this->neededQueens; i++)
    {
        blockedColor.push_back(false);
    }

    runHeuristics(result.get(), blockedX, blockedY, blockedColor);
    return this->solve(result, blockedX, blockedY, blockedColor);
}

void QueenSolver::setGridFromText(const std::string& text)
{
    int gridSize = 0;
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

    grid.clear();
    grid.reserve(gridSize);
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
        grid.push_back(vector);
    }

    // Ew... but doesn't matter:
    // We use std::move so we don't copy any data and don't lose
    // any performance
    setGrid(std::move(grid));
}

void QueenSolver::runHeuristics(
    std::vector<Vec2>* queens,
    std::vector<bool>& blockedX,
    std::vector<bool>& blockedY,
    std::vector<bool>& blockedColor
)
{
    for (std::pair<const int, std::vector<Vec2>>& colorField : colorFields)
    {
        if(colorField.second.size() == 1)
        {
            const Vec2 pos = colorField.second[0];
            queens->emplace_back(pos);
            blockedX[pos.x] = true;
            blockedY[pos.y] = true;
            blockedColor[grid[pos.y][pos.x] - 1] = true;
        }
    }

    // TODO: Pre-Calculate Single-Line colors?
}

std::unique_ptr<std::vector<Vec2>> QueenSolver::solve(
    std::unique_ptr<std::vector<Vec2>>& result,
    std::vector<bool>& blockedX,
    std::vector<bool>& blockedY,
    std::vector<bool>& blockedColor,
    int fromX,
    int fromY
)
{
    const auto size = grid.size();

    for(size_t y = fromY; y < size; y++)
    {
        if(blockedY[y])
        {
            continue;
        }

        size_t startX = 0;

        if(y == fromY)
        {
            startX = fromX;
        }

        for(size_t x = startX; x < size; x++)
        {
            const auto currentColor = this->grid[y][x] - 1;

            if(blockedColor[currentColor])
            {
                continue;
            }

            if(blockedX[x])
            {
                continue;
            }

            bool valid = true;

            for (const Vec2& queen : *result)
            {
                if (std::abs(static_cast<uint16_t>(x) - queen.x) == 1 && std::abs(static_cast<uint16_t>(y) - queen.y) == 1)
                {
                    valid = false;
                    break;
                }
            }

            if(!valid)
            {
                continue;
            }

            if(result->size() + 1 == this->neededQueens)
            {
                // we can modify the result here because we'll
                // skip out of all subtasks running after returning
                result->emplace_back(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
                return std::move(result);
            }

            result->emplace_back(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
            blockedColor[currentColor] = true;
            blockedX[x] = true;
            blockedY[y] = true;

            std::unique_ptr<std::vector<Vec2>> nextResult = this->solve(
                result,
                blockedX, 
                blockedY,
                blockedColor,
                x,
                y
            );

            if(nextResult != nullptr)
            {
                return nextResult;
            }

            result->pop_back();
            blockedColor[currentColor] = false;
            blockedX[x] = false;
            blockedY[y] = false;
        }
    }

    return nullptr;
}
