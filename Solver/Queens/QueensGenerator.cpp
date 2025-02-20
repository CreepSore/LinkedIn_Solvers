#include "QueensGenerator.h"

#include <iostream>

#include "imgui.h"

QueensGeneratorMenu::QueensGeneratorMenu()
{
    grid.initializeColors(ImVec4(1, 1, 1, 1.0));
    grid.setMouseEventHandler(
        [this](
            const uint8_t x,
            const uint8_t y,
            const uint8_t value,
            const GridMouseButton clickType,
            const GridMouseEvent eventType
        ) {
            this->onMouseEvent(x, y, value, clickType, eventType);
        }
    );

    colors.emplace_back(187.0 / 255, 163.0 / 255, 225.0 / 255, 1.0);
    colors.emplace_back(254.0 / 255, 202.0 / 255, 145.0 / 255, 1.0);
    colors.emplace_back(150.0 / 255, 189.0 / 255, 254.0 / 255, 1.0);
    colors.emplace_back(223.0 / 255, 223.0 / 255, 223.0 / 255, 1.0);
    colors.emplace_back(254.0 / 255, 123.0 / 255, 95.0 / 255, 1.0);
    colors.emplace_back(230.0 / 255, 243.0 / 255, 137.0 / 255, 1.0);
    colors.emplace_back(163.0 / 255, 210.0 / 255, 216.0 / 255, 1.0);
    colors.emplace_back(223.0 / 255, 160.0 / 255, 191.0 / 255, 1.0);
    colors.emplace_back(185.0 / 255, 178.0 / 255, 159.0 / 255, 1.0);
    colors.emplace_back(179.0 / 255, 223.0 / 255, 160.0 / 255, 1.0);
}

void QueensGeneratorMenu::render(Window* window)
{
    if (!open)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::Begin("QueensGenerator", &open, ImGuiWindowFlags_NoResize);

    renderColorBar();
    grid.render(GridRenderOptions{ .buttonSize = ImVec2(32, 32), .itemSpacing = ImVec2(1, 1), .clipboardButtons = true });

    ImGui::End();

    IRenderable::render(window);
}

void QueensGeneratorMenu::onAttach(Window* window)
{
    queensGenerator = std::make_unique<QueensGenerator>();
    IRenderable::onAttach(window);
}

void QueensGeneratorMenu::onMouseEvent(
    const size_t x,
    const size_t y,
    const uint8_t value,
    const GridMouseButton clickType,
    GridMouseEvent eventType
)
{
    if(eventType == GridMouseEvent::DOWN)
    {
        grid.getValue(x, y) = currentColor;
        grid.setColor(x, y, colors[currentColor]);
    }
}

void QueensGeneratorMenu::renderColorBar()
{
    ImGui::PushID("ColorBar");
    for(int i = 0; i < colors.size(); i++)
    {
        ImGui::PushID(i);
        ImVec4& color = colors.at(i);

        if (ImGui::Extension::ColoredButton("", color, ImVec2(25, 0)))
        {
            currentColor = i;
        }

        ImGui::SameLine();
        ImGui::PopID();
    }
    ImGui::PopID();

    ImGui::NewLine();
}
