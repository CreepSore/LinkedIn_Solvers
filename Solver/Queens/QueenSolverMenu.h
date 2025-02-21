#pragma once
#include <memory>
#include <cstdint>
#include <random>

#include "imgui.h"

#include "../UI/IRenderable.h"
#include "QueenSolver.h"
#include "../Shared/Grid.h"
#include "SFML/Graphics/RenderWindow.hpp"


class QueenSolverMenu : public IRenderable {
public:
    QueenSolverMenu();

    void onAttach(Window* window) override;
    void render(Window* window) override;
    bool open = false;

private:
    int MAX_GRID_SIZE = 20;

    uint8_t lastColor = 0;
    std::unique_ptr<Grid<>> grid = nullptr;
    std::unique_ptr<std::vector<Vec2>> result = nullptr;
    void setGridSize(int size);
    void solve();
    ImVec4 getColorForNumber(size_t color);

    void toClipboard() const;
    void fromClipboard();
    void fromText(const std::string& text);
    void attachMouseHandlerToGrid();

    std::vector<ImVec4> colors;
    std::random_device rnd;
    std::default_random_engine rndEngine;
    std::uniform_real_distribution<double> rndDistribution{ 0, 1 };
    double getRandom();
};
