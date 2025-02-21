#pragma once

#include <random>
#include <memory>
#include "../UI/IRenderable.h"
#include "../Shared/Grid.h"
#include "../Shared/Vec2.h"


class QueensGenerator {
public:
    QueensGenerator();
    std::unique_ptr<Grid<uint8_t, int32_t>> generate(uint16_t size);

private:
    std::random_device rnd;
    std::default_random_engine rndEngine;
    std::uniform_real_distribution<double> rndDistribution{0, 1};
    std::vector<ImVec4> colors;
    double getRandom();

    std::vector<bool> blockedX;
    std::vector<bool> blockedY;
    std::vector<Vec2> placed;

    std::unique_ptr<Grid<uint8_t, int32_t>> generateGrid(uint16_t size);
    void generateColors(Grid<uint8_t, int32_t>& grid);
    void generateColorsRandomWalk(
        Grid<uint8_t, int32_t>& grid,
        const Vec2& position,
        size_t colorId,
        size_t step
    );

    void generateColorsRandomWalkLinear(
        Grid<uint8_t, int32_t>& grid,
        const Vec2& position,
        size_t colorId,
        size_t step,
        int direction = -1,
        bool changedDirection = false
    );

    void fillEmptyColors(Grid<uint8_t, int32_t>& grid);
    ImVec4 getNearestColor(const Grid<uint8_t, int32_t>& grid, int32_t x, int32_t y);
    void flipColorGrid(Grid<uint8_t, int32_t>& grid) const;
    size_t colorToIndex(ImVec4& color) const;
};

class QueensGeneratorMenu : public IRenderable {
public:
    QueensGeneratorMenu();
    bool open = false;
    int32_t gridSize = 10;

    std::unique_ptr<QueensGenerator> queensGenerator = nullptr;
    std::unique_ptr<Grid<uint8_t, int32_t>> grid;

    void render(Window* window) override;
    void onAttach(Window* window) override;

private:
    int currentColor = 0;
};
