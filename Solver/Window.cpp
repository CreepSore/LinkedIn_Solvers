#include "Window.h"

#include "imgui-SFML.h"
#include "imgui.h"

bool Window::show()
{
    if(initialized)
    {
        return false;
    }

    initialized = true;

    window = sf::RenderWindow{ sf::VideoMode(sf::Vector2u(1000, 1000)), "LinkedIn Solver" };
    if (!ImGui::SFML::Init(window))
    {
        return false;
    }

    ImGui::GetIO().IniFilename = nullptr;

    return true;
}

void Window::close()
{
    ImGui::SFML::Shutdown(window);
    window.close();
}

bool Window::tick()
{
    bool cancel = false;
    while (const std::optional event = window.pollEvent())
    {
        if (event.has_value())
        {
            if (event->is<sf::Event::Closed>())
            {
                cancel = true;
                break;
            }

            ImGui::SFML::ProcessEvent(window, *event);
        }
    }

    if (cancel)
    {
        return false;
    }

    ImGui::SFML::Update(window, clock.restart());

    for (auto renderable : renderables)
    {
        renderable->render(this);
    }

    window.clear();
    ImGui::SFML::Render(window);
    window.display();

    return true;
}

void Window::addRenderable(IRenderable* renderable)
{
    renderables.insert(renderable);
    renderable->onAttach(this);
}

void Window::removeRenderable(IRenderable* renderable)
{
    renderables.erase(renderable);
}
