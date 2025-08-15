#pragma once
#include <base/common.hpp>
typedef struct Application Application;
typedef struct Player Player;
typedef struct Vec2 Vec2;

struct Application {
    SDL_Window* window;
    SDL_Renderer * renderer;
    double deltaTime;
    int fps;
    SDL_Texture* frame_texture;
    SDL_Texture* texture;
    bool debug = false;
};

struct Vec2 {
    double x, y;
    Vec2() : x(0), y(0) {}
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
    Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
    Vec2 operator*(double s)    const { return Vec2(x * s, y * s); }
    Vec2 operator/(double s)    const { return Vec2(x / s, y / s); }

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(double s)      { x *= s; y *= s; return *this; }
    Vec2& operator/=(double s)      { x /= s; y /= s; return *this; }

    double dot(const Vec2& o) const { return x * o.x + y * o.y; }
    double length() const { return std::sqrt(x * x + y * y); }
    Vec2 normalized() const { double len = length(); return len ? (*this) / len : Vec2(0, 0); }
    Vec2 rotated(double angle) const { 
        double cosA = std::cos(angle), sinA = std::sin(angle);
        return Vec2(x * cosA - y * sinA, x * sinA + y * cosA); 
    }
    Vec2 perp(double scale = 1.0) const {
        // 90-degree CCW, scaled
        return Vec2(-y * scale, x * scale);
    } 
    static Vec2 fromAngle(double angle) {
        return Vec2(std::cos(angle), std::sin(angle));
    }
};

struct Player{
    Vec2 gridPosition;
    Vec2 screenPosition;
    Vec2 projectToScreen(int cell_width, int cell_height) const {
        return Vec2(gridPosition.x * cell_width, gridPosition.y * cell_height);   
    }
    Vec2 direction;
    Vec2 camera;
    int pitch = 0;  // Vertical looking angle - positive = looking down, negative = looking up
};
