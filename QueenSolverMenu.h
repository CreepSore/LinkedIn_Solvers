#pragma once
#include "imgui.h"
#include "IRenderable.h"
#include "Solver.h"
#include "SFML/Graphics/RenderWindow.hpp"

class QueenSolverMenu : public IRenderable {
public:
    void onAttach(Window* window) override;
    void render(Window* window) override;
    bool open = false;

private:
    int MAX_GRID_SIZE = 20;

    int gridSize = 0;
    uint8_t lastColor = 0;
    std::vector<std::vector<uint8_t>> gridValues;
    std::unique_ptr<std::vector<Vec2>> result = nullptr;
    void setGridSize(int size);
    void solve();
    static ImVec4 getColorForNumber(uint8_t color);

    void toClipboard() const;
    void fromClipboard();
    void fromText(const std::string& text);
};
