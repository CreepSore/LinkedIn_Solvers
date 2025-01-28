#pragma once
#include "IRenderable.h"
#include "QueenSolver.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Menu : public IRenderable {
public:
    Menu();
    void render(Window* window) override;
    void onAttach(Window* window) override;
    void onDeattach(Window* window) override;

private:
    bool queenSolverOpen = false;
    std::shared_ptr<QueenSolver> queenSolver;
};