#pragma once
#include <iostream>

struct Vector2f
{
    Vector2f() : x(0), y(0) {}
    Vector2f(float p_x, float p_y) : x(p_x), y(p_y) {}
    float x,y;
    void print()
    {
        std::cout << x << ", " << y << "\n";
    }
};

struct Vector2i
{
    Vector2i() : x(0), y(0) {}
    Vector2i(int p_x, int p_y) : x(p_x), y(p_y) {}
    int x, y;
    void print()
    {
        std::cout << x << ", " << y << "\n";
    }
};

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}