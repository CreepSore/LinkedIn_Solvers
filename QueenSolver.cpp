#include "QueenSolver.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>

QueenSolver::QueenSolver() {

}

void QueenSolver::setGrid(std::vector<std::vector<uint8_t>> grid)
{
    this->grid = std::move(grid);

    this->neededQueens = 0;
    for(size_t x = 0; x < this->grid.size(); x++)
    {
        for(size_t y = 0; y < this->grid[x].size(); y++)
        {
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
    const std::unique_ptr<std::vector<Vec2>> result(new std::vector<Vec2>());
    std::vector<bool>blockedX{};
    std::vector<bool> blockedY{};
    std::vector<bool> blockedColor{};

    for(size_t i = 0; i < this->grid.size(); i++)
    {
        blockedX.push_back(false);
        blockedY.push_back(false);
    }

    for(size_t i = 0; i < this->neededQueens; i++)
    {
        blockedColor.push_back(false);
    }

    return this->solve(result, blockedX, blockedY, blockedColor);
}

std::unique_ptr<std::vector<Vec2>> QueenSolver::solve(
    const std::unique_ptr<std::vector<Vec2>>& result,
    std::vector<bool>& blockedX,
    std::vector<bool>& blockedY,
    std::vector<bool>& blockedColor,
    int depth
)
{
    const auto size = grid.size();

    auto canPlace = [this](
        const Vec2& position,
        std::vector<Vec2>* queens,
        std::vector<bool>& blockedX,
        std::vector<bool>& blockedY,
        std::vector<bool>& blockedColor
    ) {
        if(blockedX[position.x])
        {
            return false;
        }

        if(blockedY[position.y])
        {
            return false;
        }

        if(blockedColor[this->grid[position.y][position.x] - 1])
        {
            return false;
        }

        for (const Vec2& queen : *queens)
        {
            if (std::abs(position.x - queen.x) == 1 && std::abs(position.y - queen.y) == 1)
            {
                return false;
            }
        }

        return true;
    };

    for(size_t y = 0; y < size; y++)
    {
        for(size_t x = 0; x < size; x++)
        {
            Vec2 currentPosition{ static_cast<uint16_t>(x), static_cast<uint16_t>(y) };
            const auto currentColor = this->grid[currentPosition.y][currentPosition.x];
            if(!canPlace(currentPosition, result.get(), blockedX, blockedY, blockedColor))
            {
                continue;
            }


            std::unique_ptr<std::vector<Vec2>> nextQueens{ new std::vector(*result) };
            nextQueens->push_back(currentPosition);

            std::vector<bool> nextBlockedX{ blockedX.begin(), blockedX.end() };
            nextBlockedX[currentPosition.x] = true;

            std::vector<bool> nextBlockedY{ blockedY.begin(), blockedY.end() };
            nextBlockedY[currentPosition.y] = true;

            std::vector<bool> nextBlockedColor{ blockedColor.begin(), blockedColor.end() };
            nextBlockedColor[currentColor - 1] = true;


            if(nextQueens->size() == this->neededQueens)
            {
                bool allFilled = true;

                for(int i = 1; i <= this->neededQueens; i++)
                {
                    if(!nextBlockedColor[currentColor - 1])
                    {
                        allFilled = false;
                        break;
                    }
                }

                if(allFilled)
                {
                    return nextQueens;
                }
            }

            std::unique_ptr<std::vector<Vec2>> nextResult = this->solve(nextQueens, nextBlockedX, nextBlockedY, nextBlockedColor, depth + 1);

            if(nextResult != nullptr)
            {
                return nextResult;
            }
        }
    }

    return nullptr;
}
