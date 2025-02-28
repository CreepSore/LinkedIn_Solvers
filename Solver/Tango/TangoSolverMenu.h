#pragma once
#include <memory>
#include <cstdint>
#include "imgui.h"
#include "../UI/IRenderable.h"
#include "../Shared/Grid.h"
#include "TangoSolver.h"

#include "SFML/Graphics/RenderWindow.hpp"

class TangoSolverMenu : public IRenderable {
public:
    void onAttach(Window* window) override;
    void render(Window* window) override;
    bool open = false;

private:
    int MAX_GRID_SIZE = 15;
    uint8_t lastValue = TangoSolver::Type::FIELD_TYPE_NONE;

    int gridSize = 0;
    std::unique_ptr<Grid<char>> grid = nullptr;
    Grid<char>* result = nullptr;
    void setGridSize(int size);
    void solve();
    static ImVec4 getColorForValue(uint8_t color);

    void toClipboard() const;
    void fromClipboard();
    void fromText(const std::string& text);
    void initializeGrid();

    void gridMouseHandler(
        size_t x,
        size_t y,
        char value,
        GridMouseButton clickType,
        GridMouseEvent eventType
    );
};
