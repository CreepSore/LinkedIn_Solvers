#include "Menu.h"

#include "imgui.h"
#include "../Tango/TangoSolverMenu.h"

Menu::Menu()
{
    queenSolver = std::shared_ptr<QueenSolverMenu>{ new QueenSolverMenu() };
    tangoSolver = std::shared_ptr<TangoSolverMenu>{ new TangoSolverMenu() };
    subrenderables.push_back(queenSolver);
    subrenderables.push_back(tangoSolver);
}

void Menu::render(Window* window)
{
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("MENU", 0, ImGuiWindowFlags_NoResize);

    if(ImGui::Button("QueenSolver"))
    {
        queenSolver->open = !queenSolver->open;

        queenSolver->open
            ? queenSolver->onAttach(window)
            : queenSolver->onDeattach(window);
    }

    if(ImGui::Button("TangoSolver"))
    {
        tangoSolver->open = !tangoSolver->open;

        tangoSolver->open
            ? tangoSolver->onAttach(window)
            : tangoSolver->onDeattach(window);
    }

    ImGui::End();
    
    IRenderable::render(window);
}

void Menu::onAttach(Window* window)
{
    IRenderable::onAttach(window);
}

void Menu::onDeattach(Window* window)
{
    IRenderable::onDeattach(window);
}
