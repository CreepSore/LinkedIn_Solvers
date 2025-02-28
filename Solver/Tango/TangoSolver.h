#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <array>

#include "../Shared/Vec2.h"
#include "../Shared/Grid.h"

class TangoSolver {
public:
    enum Type : char
    {
        INVALID = '/',
        FIELD_TYPE_NONE = ' ',
        FIELD_TYPE_SUN = 'S',
        FIELD_TYPE_MOON = 'M',
        CONSTRAINT_EQUALS = '=',
        CONSTRAINT_NOT_EQUALS = 'X'
    };

    TangoSolver();
    void setGrid(Grid<char>* grid);
    Grid<char>* solve();

private:
    enum ValidType : uint8_t
    {
        VALID = 0,
        CANCEL = 1,
        COMPLETED = 2
    };

    Grid<char>* grid;
    Grid<char>* solve(
        const size_t fromX,
        const size_t fromY
    );

    std::unique_ptr<std::vector<std::array<int, 2>>> countX;
    std::unique_ptr<std::vector<std::array<int, 2>>> countY;
    bool xValid = false;
    bool yValid = false;

    bool runHeuristics(const size_t x, const size_t y) const;
    bool runCountHeuristics(const size_t x, const size_t y) const;
    bool runFieldHeuristics(const size_t x, const size_t y) const;
    bool runConstraintHeuristics(size_t x, size_t y, bool all = true) const;

    ValidType checkIsValid() const;
};
