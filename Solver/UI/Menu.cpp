#include "Menu.h"

#include "imgui.h"
#include "../Tango/TangoSolverMenu.h"

Menu::Menu()
{
    queensGenerator = std::make_shared<QueensGeneratorMenu>();
    queenSolver = std::make_shared<QueenSolverMenu>();
    tangoSolver = std::make_shared<TangoSolverMenu>();

    subrenderables.push_back(queenSolver);
    subrenderables.push_back(tangoSolver);
    subrenderables.push_back(queensGenerator);
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

    if (ImGui::Button("QueensGenerator"))
    {
        queensGenerator->open = !queensGenerator->open;

        queensGenerator->open
            ? queensGenerator->onAttach(window)
            : queensGenerator->onDeattach(window);
    }

    if (ImGui::Button("ImGui Demo"))
    {
        demoOpen = !demoOpen;
    }

    if(demoOpen)
    {
        ImGui::ShowDemoWindow(&demoOpen);
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
