#pragma once
#include <cstdint>

struct Vec2
{
    uint16_t x;
    uint16_t y;
    uint32_t hash;

    Vec2(const Vec2& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->hash = vec.hash;
    }

    Vec2(uint16_t x, uint16_t y)
    {
        this->x = x;
        this->y = y;
        this->hash = (x << sizeof(decltype(x)) * 8) + this->y;
    }

    bool operator<(const Vec2& v2) const noexcept
    {
        return this->hash - v2.hash < 0;
    }
};
