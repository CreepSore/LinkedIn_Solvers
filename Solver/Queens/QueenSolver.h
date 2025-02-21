#pragma once
#include <vector>
#include <format>
#include "../Shared/Vec2.h"
#include <memory>
#include <cstdint>
#include <cmath>
#include <map>


class QueenSolver {
public:
    QueenSolver();
    void setGrid(std::vector<std::vector<uint8_t>> grid);
    std::vector<std::vector<uint8_t>>& getGrid();
    std::unique_ptr<std::vector<Vec2>> solve();
    void setGridFromText(const std::string& text);

private:
    std::map<int, std::vector<Vec2>> colorFields;

    void runHeuristics(
        std::vector<Vec2>* queens,
        std::vector<bool>& blockedX,
        std::vector<bool>& blockedY,
        std::vector<bool>& blockedColor
    );

    std::unique_ptr<std::vector<Vec2>> solve(
        std::unique_ptr<std::vector<Vec2>>& result,
        std::vector<bool>& blockedX,
        std::vector<bool>& blockedY,
        std::vector<bool>& blockedColor,
        int fromX = 0,
        int fromY = 0
    );
    std::vector<std::vector<uint8_t>> grid;
    int neededQueens = 0;
};
