#pragma once

#include <memory>
#include "../UI/IRenderable.h"
#include "../Shared/Grid.h"


class QueensGenerator {
public:
};

class QueensGeneratorMenu : public IRenderable {
public:
    QueensGeneratorMenu();
    bool open = false;

    std::unique_ptr<QueensGenerator> queensGenerator = nullptr;
    Grid<uint8_t> grid{10, 10, 0};

    void render(Window* window) override;
    void onAttach(Window* window) override;

    void onMouseEvent(size_t x, size_t y, uint8_t value, GridMouseButton clickType, GridMouseEvent eventType);

private:
    int currentColor = 0;
    std::vector<ImVec4> colors;
    void renderColorBar();
};
