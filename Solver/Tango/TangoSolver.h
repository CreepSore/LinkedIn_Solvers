#pragma once
#include <vector>
#include <format>
#include <memory>

#include "../Shared/Vec2.h"

class TangoSolver {
public:
    enum Type : uint8_t
    {
        NONE = ' ',
        SUN = 'S',
        MOON = 'M'
    };

    TangoSolver();
    void setGrid(std::vector<std::vector<uint8_t>> grid);
    std::vector<std::vector<uint8_t>>& getGrid();
    std::unique_ptr<std::vector<std::vector<uint8_t>>> solve();
    void setGridFromText(const std::string& text);

private:
    std::unique_ptr<std::vector<std::vector<uint8_t>>> solve(std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid);
    std::vector<std::vector<uint8_t>> grid;
    static Type heuristics(const std::unique_ptr<std::vector<std::vector<uint8_t>>>& grid, size_t x, size_t y);
};
