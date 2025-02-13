#pragma once
#include "imgui.h"
#include "../UI/IRenderable.h"
#include "TangoSolver.h"
#include "SFML/Graphics/RenderWindow.hpp"

class TangoSolverMenu : public IRenderable {
public:
    void onAttach(Window* window) override;
    void render(Window* window) override;
    bool open = false;

private:
    int MAX_GRID_SIZE = 15;
    uint8_t lastValue = TangoSolver::Type::NONE;

    int gridSize = 0;
    std::vector<std::vector<uint8_t>> gridValues;
    std::unique_ptr<std::vector<std::vector<uint8_t>>> result = nullptr;
    void setGridSize(int size);
    void solve();
    static ImVec4 getColorForValue(uint8_t color);

    void toClipboard() const;
    void fromClipboard();
    void fromText(const std::string& text);
};
