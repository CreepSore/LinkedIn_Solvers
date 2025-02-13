#pragma once
#include <set>

#include "SFML/Graphics/RenderWindow.hpp"
#include "IRenderable.h"

class Window {
public:
    bool show();
    void close();
    bool tick();
    void addRenderable(IRenderable* renderable);
    void removeRenderable(IRenderable* renderable);

private:
    sf::Clock clock;
    bool initialized = false;
    sf::RenderWindow window;
    std::set<IRenderable*> renderables;
};
