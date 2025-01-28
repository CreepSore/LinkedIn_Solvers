#include "main.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "Menu.h"
#include "Window.h"

struct CliArguments
{
    std::string queensInput;
};

int initUi()
{
    Menu menu{};
    Window window;
    if(!window.show())
    {
        return -1;
    }

    window.addRenderable(&menu);

    while(true)
    {
        if(!window.tick())
        {
            break;
        }
    }

    return 0;
}

CliArguments parseCliArguments(int argc, char* argv[])
{
    enum class ParseMode
    {
        NONE = 0,
        QUEENSINPUT = 1
    };

    CliArguments result{};
    ParseMode parseMode = ParseMode::NONE;
    

    for(int i = 0; i < argc; i++)
    {
        switch(parseMode)
        {
        case ParseMode::NONE:
            if (std::string(argv[i]) == "-queens")
            {
                parseMode = ParseMode::QUEENSINPUT;
            }

            break;

        case ParseMode::QUEENSINPUT:
            result.queensInput = argv[i];
            parseMode = ParseMode::NONE;
            break;
        }
    }

    return result;
}

int main(int argc, char* argv[])
{
    CliArguments arguments = parseCliArguments(argc, argv);

    if(!arguments.queensInput.empty())
    {
        QueenSolver solver{};
        solver.setGridFromText(arguments.queensInput);
        auto result = solver.solve();

        if (result == nullptr) {
            std::cout << "No solution found!\n\n";
            return -1;
        }

        for (auto queen : *result)
        {
            std::cout << queen.x << "x" << queen.y << "y\n";
        }

        std::cout << "\n";

        return 0;
    }


    return initUi();
}
