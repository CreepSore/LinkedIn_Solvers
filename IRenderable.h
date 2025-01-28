#pragma once
#include <memory>
#include <set>
#include <vector>

class Window;

class IRenderable {
public:
    std::set<Window*> windows;
    std::vector<std::shared_ptr<IRenderable>> subrenderables;

    virtual ~IRenderable() = default;
    virtual void render(Window* window)
    {
        for (auto& subrenderable : subrenderables)
        {
            subrenderable->render(window);
        }
    }

    virtual void onAttach(Window* window)
    {
        windows.insert(window);

        for (auto& subrenderable : subrenderables)
        {
            subrenderable->onAttach(window);
        }
    }

    virtual void onDeattach(Window* window)
    {
        windows.erase(window);

        for (auto& subrenderable : subrenderables)
        {
            subrenderable->onDeattach(window);
        }
    }

    void reattach()
    {
        for (auto window : windows)
        {
            IRenderable::onAttach(window);
            IRenderable::onDeattach(window);
        }
    }
};
