#include "TangoSolver.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>

TangoSolver::TangoSolver() {

}

void TangoSolver::setGrid(std::vector<std::vector<uint8_t>> grid)
{
    this->grid = grid;
}

std::vector<std::vector<uint8_t>>& TangoSolver::getGrid()
{
    return this->grid;
}

std::unique_ptr<std::vector<std::vector<uint8_t>>> TangoSolver::solve()
{
    std::unique_ptr result(std::make_unique<std::vector<std::vector<uint8_t>>>());
    const size_t size = this->grid.size();

    result->reserve(size);
    for(size_t i = 0; i < size; i++)
    {
        std::vector<uint8_t> row;
        row.reserve(size);
        for(size_t j = 0; j < size; j++)
        {
            row.push_back(this->grid[i][j]);
        }

        result->push_back(row);
    }

    bool retry = true;
    while(retry)
    {
        retry = false;
        for(uint8_t y = 0; y < size; y++)
        {
            for (uint8_t x = 0; x < size; x++)
            {
                auto heuristic = heuristics(result, x, y);
                if (heuristic != NONE) {
                    retry = true;
                    (*result)[y][x] = heuristic;
                }
            }
        }
    }

    return this->solve(result);
}

void TangoSolver::setGridFromText(const std::string& text)
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

std::unique_ptr<std::vector<std::vector<uint8_t>>> TangoSolver::solve(std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid)
{
    const auto size = grid->size();

    const auto checkGrid = [](const std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid) {
        const auto checkRow = [](const std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid, size_t rownum) {
            const size_t size = grid->size();
            bool noMaxConcurrent = true;
            std::map<Type, uint8_t> total {
                {NONE, 0},
                {MOON, 0},
                {SUN, 0}
            };

            for(size_t i = 2; i < size; i++)
            {
                if(i == 2)
                {
                    for(size_t j = 0; j < i; j++)
                    {
                        uint8_t value = (*grid)[rownum][j];
                        total[static_cast<Type>(value)] += 1;
                    }
                }

                uint8_t value = (*grid)[rownum][i];
                total[static_cast<Type>(value)] += 1;

                if((*grid)[rownum][i] != NONE && (*grid)[rownum][i] == (*grid)[rownum][i-1] && (*grid)[rownum][i] == (*grid)[rownum][i-2])
                {
                    noMaxConcurrent = false;
                }

                if (!noMaxConcurrent && total[NONE] > 0)
                {
                    break;
                }
            }

            return std::make_tuple(total[MOON] == total[SUN], noMaxConcurrent);
        };

        const auto checkColumn = [](const std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid, size_t colnum) {
            const size_t size = grid->size();
            bool noMaxConcurrent = true;
            std::map<Type, uint8_t> total{
                {NONE, 0},
                {MOON, 0},
                {SUN, 0}
            };

            for (size_t i = 2; i < size; i++)
            {
                if (i == 2)
                {
                    for (size_t j = 0; j < i; j++)
                    {
                        uint8_t value = (*grid)[j][colnum];
                        total[static_cast<Type>(value)] += 1;
                    }
                }

                uint8_t value = (*grid)[i][colnum];
                total[static_cast<Type>(value)] += 1;

                if ((*grid)[i][colnum] != NONE && (*grid)[i][colnum] == (*grid)[i - 1][colnum] && (*grid)[i][colnum] == (*grid)[i - 2][colnum])
                {
                    noMaxConcurrent = false;
                }

                if(!noMaxConcurrent && total[NONE] > 0)
                {
                    break;
                }
            }

            return std::make_tuple(total[MOON] == total[SUN], noMaxConcurrent);
        };

        const size_t size = grid->size();
        auto valid = std::make_tuple(true, true);

        for(size_t i = 0; i < size; i++)
        {
            auto rowCheck = checkRow(grid, i);
            auto colCheck = checkColumn(grid, i);

            if(!std::get<0>(rowCheck) || !std::get<0>(colCheck))
            {
                std::get<0>(valid) = false;
            }

            if (!std::get<1>(rowCheck) || !std::get<1>(colCheck))
            {
                std::get<1>(valid) = false;
            }

            if(!std::get<0>(valid) && !std::get<1>(valid))
            {
                break;
            }
        }

        return valid;
    };

    auto gridCheck = checkGrid(grid);

    if(std::get<0>(gridCheck) && std::get<1>(gridCheck))
    {
        return std::move(grid);
    }

    if(!std::get<1>(gridCheck))
    {
        return nullptr;
    }

    for (size_t y = 0; y < size; y++)
    {
        for (size_t x = 0; x < size; x++)
        {
            Type value = static_cast<Type>((*grid)[y][x]);
            if(value != NONE)
            {
                continue;
            }

            std::unique_ptr<std::vector<std::vector<uint8_t>>> newGrid{ new std::vector<std::vector<uint8_t>>() };
            newGrid->reserve(size);
            for(size_t y2 = 0; y2 < size; y2++)
            {
                std::vector<uint8_t> row;
                row.reserve(size);
                for(size_t x2 = 0; x2 < size; x2++) {
                    row.push_back((*grid)[y2][x2]);
                }
                newGrid->push_back(row);
            }


            auto heuristic = heuristics(newGrid, x, y);
            bool match = false;
            bool retry = false;

            if (heuristic != NONE) {
                match = true;
                retry = true;
                (*newGrid)[y][x] = heuristic;
            }

            while (retry)
            {
                retry = false;
                for (uint8_t y = 0; y < size; y++)
                {
                    for (uint8_t x = 0; x < size; x++)
                    {
                        heuristic = heuristics(newGrid, x, y);
                        if (heuristic != NONE) {
                            retry = true;
                            match = true;
                            (*newGrid)[y][x] = heuristic;
                        }
                    }
                }
            }

            if(match)
            {
                auto heuristicResult = this->solve(newGrid);
                if (heuristicResult != nullptr)
                {
                    return std::move(heuristicResult);
                }
                continue;
            }

            (*newGrid)[y][x] = SUN;
            auto sunResult = this->solve(newGrid);
            if(sunResult != nullptr)
            {
                return std::move(sunResult);
            }

            (*newGrid)[y][x] = MOON;
            auto moonResult = this->solve(newGrid);
            if(moonResult != nullptr)
            {
                return std::move(moonResult);
            }
        }
    }

    return nullptr;
}

TangoSolver::Type TangoSolver::heuristics(
    const std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid,
    size_t x,
    size_t y
)
{
    if((*grid)[y][x] != NONE)
    {
        return NONE;
    }

    if (y >= 1 && y + 1 < grid->size() && (*grid)[y - 1][x] != NONE && (*grid)[y - 1][x] == (*grid)[y + 1][x])
    {
        return (*grid)[y - 1][x] == SUN ? MOON : SUN;
    }

    if (x >= 1 && x + 1 < grid->size() && (*grid)[y][x - 1] != NONE && (*grid)[y][x - 1] == (*grid)[y][x + 1])
    {
        return (*grid)[y][x - 1] == SUN ? MOON : SUN;
    }

    return NONE;
}
