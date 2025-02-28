#include "TangoSolver.h"

#include <utility>

TangoSolver::TangoSolver() {

}

void TangoSolver::setGrid(Grid<char>* grid)
{
    this->grid = grid;
}

Grid<char>* TangoSolver::solve()
{
    const auto width = grid->getWidth();
    const auto height = grid->getHeight();

    countX = std::make_unique<std::vector<std::array<int, 2>>>();
    countX->reserve(width);

    countY = std::make_unique<std::vector<std::array<int, 2>>>();
    countY->reserve(height);

    for (size_t i = 0; i < width; i++)
    {
        countX->push_back(std::array<int, 2>({ 0, 0 }));
        countY->push_back(std::array<int, 2>({ 0, 0 }));
    }

    for (size_t y = 0; y < height; y += 2)
    {
        for (size_t x = 0; x < width; x += 2)
        {
            const auto value = grid->getValue(x, y);

            if (value == FIELD_TYPE_SUN)
            {
                (*countX)[x][0] += 1;
                (*countY)[y][0] += 1;
            }
            else if (value == FIELD_TYPE_MOON)
            {
                (*countX)[x][1] += 1;
                (*countY)[y][1] += 1;
            }
        }
    }

    bool run = true;
    while(run)
    {
        run = false;

        for(size_t y = 0; y < height; y += 2)
        {
            for(size_t x = 0; x < width; x += 2)
            {
                auto value = grid->getValue(x, y);

                if (value != FIELD_TYPE_NONE)
                {
                    continue;
                }

                if(!runHeuristics(x, y))
                {
                    continue;
                }

                value = grid->getValue(x, y);
                run = true;

                if (value == FIELD_TYPE_SUN)
                {
                    (*countX)[x][0] += 1;
                    (*countY)[y][0] += 1;
                }
                else if (value == FIELD_TYPE_MOON)
                {
                    (*countX)[x][1] += 1;
                    (*countY)[y][1] += 1;
                }
            }
        }
    }

    return solve(0, 0);
}

Grid<char>* TangoSolver::solve(const size_t fromX, const size_t fromY)
{
    size_t currentFromX = fromX;

    for (size_t y = fromY; y < grid->getHeight(); y += 2)
    {
        for (size_t x = currentFromX; x < grid->getWidth(); x += 2)
        {
            const auto currentValue = grid->getValue(x, y);

            if(currentValue != FIELD_TYPE_NONE)
            {
                const char top = y > 0
                    ? grid->getValue(x, y - 2)
                    : INVALID;

                const char left = x > 0
                    ? grid->getValue(x - 2, y)
                    : INVALID;

                const auto topTop = y >= 4
                    ? grid->getValue(x, y - 4)
                    : INVALID;

                const auto leftLeft = x >= 4
                    ? grid->getValue(x - 4, y)
                    : INVALID;

                if (top != INVALID && top == topTop && top == currentValue)
                {
                    return nullptr;
                }

                if(left != INVALID && left == leftLeft && left == currentValue)
                {
                    return nullptr;
                }

                continue;
            }

            switch (checkIsValid())
            {
            case VALID:
                break;

            case COMPLETED:
                return grid;

            case CANCEL:
                return nullptr;
            }

            if (runConstraintHeuristics(x, y, false))
            {
                const auto newvalue = grid->getValue(x, y);
                if(newvalue == FIELD_TYPE_SUN)
                {
                    (*countX)[x][0] += 1;
                    (*countY)[y][0] += 1;
                }
                else
                {
                    (*countX)[x][1] += 1;
                    (*countY)[y][1] += 1;
                }

                auto resultHeuristics = solve(x, y);
                if(resultHeuristics)
                {
                    return resultHeuristics;
                }

                if (newvalue == FIELD_TYPE_SUN)
                {
                    (*countX)[x][0] -= 1;
                    (*countY)[y][0] -= 1;
                }
                else
                {
                    (*countX)[x][1] -= 1;
                    (*countY)[y][1] -= 1;
                }
                grid->setValue(x, y, currentValue);

                continue;
            }

            grid->setValue(x, y, FIELD_TYPE_SUN);
            (*countX)[x][0] += 1;
            (*countY)[y][0] += 1;
            auto resultSun = solve(x, y);
            if(resultSun != nullptr)
            {
                return resultSun;
            }
            (*countX)[x][0] -= 1;
            (*countY)[y][0] -= 1;


            grid->setValue(x, y, FIELD_TYPE_MOON);
            (*countX)[x][1] += 1;
            (*countY)[y][1] += 1;
            auto resultMoon = solve(x, y);
            if (resultMoon != nullptr)
            {
                return resultMoon;
            }
            (*countX)[x][1] -= 1;
            (*countY)[y][1] -= 1;

            grid->setValue(x, y, FIELD_TYPE_NONE);
        }

        currentFromX = 0;
    }

    if(checkIsValid() == COMPLETED)
    {
        return grid;
    }

    return nullptr;
}

bool TangoSolver::runHeuristics(const size_t x, const size_t y) const
{
    if(runFieldHeuristics(x, y))
    {
        //grid->setColor(x, y, ImVec4(1, 0, 0, 1));
        return true;
    }

    if(runConstraintHeuristics(x, y))
    {
        //grid->setColor(x, y, ImVec4(0, 1, 0, 1));
        return true;
    }

    if(runCountHeuristics(x, y))
    {
        //grid->setColor(x, y, ImVec4(0, 0, 1, 1));
        return true;
    }

    return false;
}

bool TangoSolver::runCountHeuristics(const size_t x, const size_t y) const
{
    const auto width = grid->getWidth();
    const auto realWidth = static_cast<size_t>(std::floor((1.0 + width) * 0.5));
    const auto neededSymbols = static_cast<int>(std::floor(realWidth * 0.5));

    if(
        ((*countX)[x][0] == neededSymbols && (*countX)[x][1] == neededSymbols - 1)
        || ((*countX)[x][1] == neededSymbols && (*countX)[x][0] == neededSymbols - 1)
    )
    {
        if((*countX)[x][0] < (*countX)[x][1])
        {
            grid->setValue(x, y, FIELD_TYPE_SUN);
            return true;
        }

        grid->setValue(x, y, FIELD_TYPE_MOON); 
        return true;
    }

    if (
        ((*countY)[y][0] == neededSymbols && (*countY)[y][1] == neededSymbols - 1)
        || ((*countY)[y][1] == neededSymbols && (*countY)[y][0] == neededSymbols - 1)
    )
    {
        if ((*countY)[y][0] < (*countY)[y][1])
        {
            grid->setValue(x, y, FIELD_TYPE_SUN);
            return true;
        }

        grid->setValue(x, y, FIELD_TYPE_MOON);
        return true;
    }

    return false;
}

bool TangoSolver::runFieldHeuristics(const size_t x, const size_t y) const
{
    const auto current = grid->getValue(x, y);

    if(current != FIELD_TYPE_NONE)
    {
        return false;
    }

    const char top = y > 0
        ? grid->getValue(x, y - 2)
        : INVALID;

    const char right = x + 2 < grid->getWidth()
        ? grid->getValue(x + 2, y)
        : INVALID;

    const char bottom = y + 2 < grid->getHeight()
        ? grid->getValue(x, y + 2)
        : INVALID;

    const char left = x > 0
        ? grid->getValue(x - 2, y)
        : INVALID;

    // We save one comparison by not checking the right value if it's also invalid
    if (top != INVALID && top != FIELD_TYPE_NONE && top == bottom)
    {
        grid->setValue(x, y, top == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    if (left != INVALID && left != FIELD_TYPE_NONE && left == right)
    {
        grid->setValue(x, y, left == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    const auto topTop = y >= 4
        ? grid->getValue(x, y - 4)
        : INVALID;

    if (topTop != INVALID && topTop != FIELD_TYPE_NONE && topTop == top)
    {
        grid->setValue(x, y, top == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    const auto leftLeft = x >= 4
        ? grid->getValue(x - 4, y)
        : INVALID;

    if(leftLeft != INVALID && leftLeft != FIELD_TYPE_NONE && leftLeft == left)
    {
        grid->setValue(x, y, left == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    const auto rightRight = x + 4 < grid->getWidth()
        ? grid->getValue(x + 4, y)
        : INVALID;

    if (rightRight != INVALID && rightRight != FIELD_TYPE_NONE && rightRight == right)
    {
        grid->setValue(x, y, right == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    const auto bottomBottom = y + 4 < grid->getHeight()
        ? grid->getValue(x, y + 4)
        : INVALID;

    if (bottomBottom != INVALID && bottomBottom != FIELD_TYPE_NONE && bottomBottom == bottom)
    {
        grid->setValue(x, y, bottom == FIELD_TYPE_SUN ? FIELD_TYPE_MOON : FIELD_TYPE_SUN);
        return true;
    }

    return false;
}

bool TangoSolver::runConstraintHeuristics(const size_t x, const size_t y, bool all) const
{
    const auto current = grid->getValue(x, y);

    if (current != FIELD_TYPE_NONE)
    {
        return false;
    }

    const auto constraintLeft = x != 0
        ? grid->getValue(x - 1, y)
        : INVALID;

    if(constraintLeft != INVALID)
    {
        const auto left = grid->getValue(x - 2, y);

        if(constraintLeft == CONSTRAINT_EQUALS)
        {
            if(left != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, left);
                return true;   
            }
        }
        else if(constraintLeft == CONSTRAINT_NOT_EQUALS)
        {
            if (left != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, left == FIELD_TYPE_MOON ? FIELD_TYPE_SUN : FIELD_TYPE_MOON);
                return true;
            }
        }
    }

    const auto constraintTop = y != 0
        ? grid->getValue(x, y - 1)
        : INVALID;

    if(constraintTop != INVALID)
    {
        const auto top = grid->getValue(x, y - 2);

        if (constraintTop == CONSTRAINT_EQUALS)
        {
            if (top != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, top);
                return true;
            }
        }
        else if (constraintTop == CONSTRAINT_NOT_EQUALS)
        {
            if (top != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, top == FIELD_TYPE_MOON ? FIELD_TYPE_SUN : FIELD_TYPE_MOON);
                return true;
            }
        }
    }

    if(!all)
    {
        return false;
    }

    const auto constraintRight = x + 1 < grid->getWidth()
        ? grid->getValue(x + 1, y)
        : INVALID;

    if (constraintRight != INVALID)
    {
        const auto right = grid->getValue(x + 2, y);

        if (constraintRight == CONSTRAINT_EQUALS)
        {
            if (right != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, right);
                return true;
            }
        }
        else if (constraintRight == CONSTRAINT_NOT_EQUALS)
        {
            if (right != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, right == FIELD_TYPE_MOON ? FIELD_TYPE_SUN : FIELD_TYPE_MOON);
                return true;
            }
        }
    }

    const auto constraintBottom = y + 1 < grid->getHeight()
        ? grid->getValue(x, y + 1)
        : INVALID;

    if (constraintBottom != INVALID)
    {
        const auto bottom = grid->getValue(x, y + 2);

        if (constraintBottom == CONSTRAINT_EQUALS)
        {
            if (bottom != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, bottom);
                return true;
            }
        }
        else if (constraintBottom == CONSTRAINT_NOT_EQUALS)
        {
            if (bottom != FIELD_TYPE_NONE)
            {
                grid->setValue(x, y, bottom == FIELD_TYPE_MOON ? FIELD_TYPE_SUN : FIELD_TYPE_MOON);
                return true;
            }
        }
    }

    return false;
}

TangoSolver::ValidType TangoSolver::checkIsValid() const
{
    const auto width = grid->getWidth();
    const auto realWidth  = static_cast<size_t>(std::floor((1.0 + width) * 0.5));
    const auto neededSymbols = static_cast<int>(std::floor(realWidth * 0.5));

    bool completed = true;

    for(size_t i = 0; i < width; i += 2)
    {
        if(
            (*countX)[i][0] > neededSymbols
            || (*countX)[i][1] > neededSymbols
            || (*countY)[i][0] > neededSymbols
            || (*countY)[i][1] > neededSymbols
        )
        {
            return CANCEL;
        }

        if(
            (*countX)[i][0] != (*countX)[i][1]
            || (*countY)[i][0] != (*countY)[i][1]
            || (*countX)[i][0] != neededSymbols
            || (*countY)[i][0] != neededSymbols
        )
        {
            completed = false;
        }
    }

    if(completed)
    {
        return COMPLETED;
    }

    return VALID;
}
