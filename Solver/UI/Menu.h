#pragma once
#include "IRenderable.h"
#include "../Queens/QueenSolverMenu.h"
#include "SFML/Window.hpp"

class TangoSolverMenu;

class Menu : public IRenderable {
public:
    Menu();
    void render(Window* window) override;
    void onAttach(Window* window) override;
    void onDeattach(Window* window) override;

private:
    std::shared_ptr<QueenSolverMenu> queenSolver = nullptr;
    std::shared_ptr<TangoSolverMenu> tangoSolver = nullptr;
};
