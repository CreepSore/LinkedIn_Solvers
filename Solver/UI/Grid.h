#pragma once
#include <functional>
#include <sstream>
#include <string>

#include "imgui.h"
#include "ImGuiExtensions.h"

enum class GridMouseButton : uint8_t
{
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2,
    EXTRA_1 = 3,
    EXTRA_2 = 4
};

enum class GridMouseEvent : uint8_t
{
    CLICK = 0,
    DOWN = 1
};

template <typename ValueType = uint8_t, typename SizeType = size_t>
struct Grid {
    typedef std::function<void(
        SizeType x,
        SizeType y,
        ValueType value,
        GridMouseButton clickType,
        GridMouseEvent eventType
    )> GridMouseEventHandler;

    std::vector<std::vector<ValueType>> rawGrid;
    ValueType defaultValue;
    bool hideLabels = false;

    Grid(
        SizeType width,
        SizeType height,
        ValueType defaultValue,
        const GridMouseEventHandler mouseEventHandler = nullptr
    )
    {
        this->width = width;
        this->height = height;
        this->defaultValue = defaultValue;
        reset();
        setMouseEventHandler(mouseEventHandler);
    }

    Grid(
        decltype(rawGrid)& grid,
        const GridMouseEventHandler mouseEventHandler = nullptr
    )
    {
        if(grid.size() == 0)
        {
            this->width = 0;
            this->height = 0;
            this->rawGrid.clear();
            return;
        }

        this->height = grid.size();
        this->width = grid[0].size();
        this->defaultValue = 0;
        this->rawGrid = grid;
        setMouseEventHandler(mouseEventHandler);
    }

    void setValue(SizeType x, SizeType y, ValueType value)
    {
        rawGrid[y][x] = std::move(value);
    }

    ValueType& getValue(SizeType x, SizeType y)
    {
        return rawGrid[y][x];
    }

    SizeType getWidth() const noexcept
    {
        return width;
    }

    SizeType getHeight() const noexcept
    {
        return height;
    }

    void reset()
    {
        rawGrid.clear();
        rawGrid.reserve(height);

        for (SizeType y = 0; y < height; ++y)
        {
            std::vector<ValueType> row;
            row.reserve(width);
            for (SizeType x = 0; x < width; ++x)
            {
                row.push_back(defaultValue);
            }
            rawGrid.push_back(row);
        }
    }

    void setMouseEventHandler(
        GridMouseEventHandler mouseEventHandler
    ) noexcept
    {
        this->mouseEventHandler = mouseEventHandler;
    }

    std::unique_ptr<std::vector<std::vector<ValueType>>> getResult()
    {
        std::unique_ptr<std::vector<std::vector<ValueType>>> result = std::make_unique<std::vector<std::vector<ValueType>>>();
        result->reserve(height);

        for (SizeType y = 0; y < height; ++y)
        {
            std::vector<ValueType> row;
            row.reserve(width);
            for (SizeType x = 0; x < width; ++x)
            {
                row.push_back(getValue(x, y));
            }
            result->push_back(row);
        }

        return result;
    }

    void render(ImVec2 buttonSize = ImVec2(-1, -1), ImVec2 itemSpacing = ImVec2(-1, -1)) const
    {
        ImGuiStyle& style = ImGui::GetStyle();

        ImVec2 vecButtonSize = buttonSize.x != -1
            ? buttonSize
            : ImVec2(32, 32);

        ImVec2 vecItemSpacing = itemSpacing.x != -1
            ? itemSpacing
            : style.ItemSpacing;

        ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, vecItemSpacing.x);
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, vecItemSpacing.y);

        for (SizeType y = 0; y < height; ++y)
        {
            ImGui::PushID(y);
            ImGui::NewLine();
            for (SizeType x = 0; x < width; ++x)
            {
                ImGui::PushID(x);
                ValueType value = rawGrid[y][x];
                const std::string label = !hideLabels
                    ? std::to_string(value)
                    : std::string(" ");

                ImVec4 buttonColor = ImVec4(-1, -1, -1, -1);

                if (mouseEventHandler == nullptr)
                {
                    ImGui::BeginDisabled();
                }

                if(colorGrid != nullptr)
                {
                    buttonColor = colorGrid->getValue(x, y);

                    if(buttonColor.x != -1)
                    {
                        ImGui::Extension::ColoredButton(label.data(), buttonColor, vecButtonSize);
                    }
                }

                if(buttonColor.x == -1)
                {
                    ImGui::Button(label.data(), vecButtonSize);
                }

                fireClickHandlers(x, y, value);
                fireDownHandlers(x, y, value);


                if (mouseEventHandler == nullptr)
                {
                    ImGui::EndDisabled();
                }

                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::PopID();
        }

        ImGui::PopStyleVar(2);
    }

    void setColor(SizeType x, SizeType y, ImVec4 color = ImVec4(-1, -1, -1, -1))
    {
        if(colorGrid == nullptr)
        {
            colorGrid = std::make_unique<Grid<ImVec4, SizeType>>(Grid<ImVec4, SizeType>(width, height, ImVec4(-1, -1, -1, -1)));
        }

        colorGrid->setValue(x, y, std::move(color));
    }

    ImVec4& getColor(SizeType x, SizeType y) const
    {
        return colorGrid->getValue(x, y);
    }

    void initializeColors(ImVec4 color)
    {
        if (colorGrid == nullptr)
        {
            colorGrid = std::make_unique<Grid<ImVec4, SizeType>>(Grid<ImVec4, SizeType>(width, height, color));
            return;
        }

        colorGrid->defaultValue = color;
        colorGrid->reset();
    }

    std::string toString()
    {
        std::stringstream ss;

        for (int y = 0; y < height; y++)
        {
            if (y != 0)
            {
                ss << "\n";
            }

            for (int x = 0; x < width; x++)
            {
                if (x != 0) {
                    ss << ',';
                }

                ss << std::to_string(rawGrid[y][x]);
            }
        }

        ss << '\n';

        return ss.str();
    }

    static std::unique_ptr<Grid> fromString(const std::string& str, GridMouseEventHandler mouseEventHandler = nullptr)
    {
        decltype(rawGrid) grid;
        size_t width = 0;

        std::string input;
        std::vector<ValueType> row;
        for(size_t i = 0; i < str.size(); i++)
        {
            const char c = str[i];

            if(c == '\r')
            {
                continue;
            }

            if(c == ',' || c == '\n')
            {
                row.push_back(std::stol(input));
                input.clear();

                if(c == '\n' && !row.empty())
                {
                    grid.push_back(row);
                    if (width == 0)
                    {
                        width = i;
                    }

                    row = std::vector<ValueType>();
                    if (width != 0)
                    {
                        row.reserve(width);
                    }
                }
            }
            else
            {
                input.push_back(c);
            }
        }

        if(!input.empty())
        {
            row.push_back(std::stol(input));
        }

        if(!row.empty() && row.size() == width)
        {
            grid.push_back(row);
        }

        return std::make_unique<Grid>(grid, mouseEventHandler);
    }

private:
    SizeType width = 0;
    SizeType height = 0;

    std::unique_ptr<Grid<ImVec4, SizeType>> colorGrid = nullptr;
    GridMouseEventHandler mouseEventHandler = nullptr;

    void fireClickHandlers(SizeType x, SizeType y, ValueType& value) const
    {
        if(
            mouseEventHandler == nullptr
            || !ImGui::IsItemHovered()
        )
        {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();

        for(int i = 0; i < sizeof(io.MouseClicked); i++)
        {
            if (!io.MouseClicked[i])
            {
                continue;
            }

            mouseEventHandler(x, y, value, static_cast<GridMouseButton>(i), GridMouseEvent::CLICK);
        }
    }

    void fireDownHandlers(SizeType x, SizeType y, ValueType& value) const
    {
        if (
            mouseEventHandler == nullptr
            || !ImGui::IsItemHovered()
            )
        {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();

        for (int i = 0; i < sizeof(io.MouseDown); i++)
        {
            if(!io.MouseDown[i])
            {
                continue;
            }

            mouseEventHandler(x, y, value, static_cast<GridMouseButton>(i), GridMouseEvent::DOWN);
        }
    }
};
