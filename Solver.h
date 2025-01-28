#pragma once
#include <vector>
#include <format>
#include <map>
#include <set>

struct Vec2
{
    uint16_t x;
    uint16_t y;
    uint32_t hash;

    Vec2(const Vec2& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->hash = vec.hash;
    }

    Vec2(uint16_t x, uint16_t y)
    {
        this->x = x;
        this->y = y;
        this->hash = (x << sizeof(decltype(x)) * 8) + this->y;
    }

    bool operator<(const Vec2& v2) const noexcept
    {
        return this->hash - v2.hash < 0;
    }
};

class Solver {
public:
    Solver();
    void setGrid(std::vector<std::vector<uint8_t>> grid);
    std::vector<std::vector<uint8_t>>& getGrid();
    std::unique_ptr<std::vector<Vec2>> solve();

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
