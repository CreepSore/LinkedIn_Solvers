#include "Menu.h"

#include "imgui.h"

Menu::Menu()
{
    queenSolver = std::shared_ptr<QueenSolverMenu>{ new QueenSolverMenu() };
    subrenderables.push_back(queenSolver);
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
