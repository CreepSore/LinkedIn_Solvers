#pragma once
#include <vector>
#include <format>
#include "../Shared/Vec2.h"


class QueenSolver {
public:
    QueenSolver();
    void setGrid(std::vector<std::vector<uint8_t>> grid);
    std::vector<std::vector<uint8_t>>& getGrid();
    std::unique_ptr<std::vector<Vec2>> solve();
    void setGridFromText(const std::string& text);

private:
    std::unique_ptr<std::vector<Vec2>> solve(
        const std::unique_ptr<std::vector<Vec2>>& result,
        std::vector<bool>& blockedX,
        std::vector<bool>& blockedY,
        std::vector<bool>& blockedColor,
        int depth = 0 // for debugging purposes
    );
    std::vector<std::vector<uint8_t>> grid;
    int neededQueens = 0;
};
