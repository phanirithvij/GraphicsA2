#pragma once
#include "main.h"

// #ifndef Plane_H
// #define Plane_H


class Arrow {
public:
    Arrow() {}
    Arrow(float x, float y, float z, color_t color);
    double speed;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 points_toward;
    void set_position(float x, float y, float z);
    void point_toward(glm::vec3 targetpos);
    void tick(float offset);
    void draw(glm::mat4 VP);
    void scale(float scl);
    void debug_point(int I, int J, int K);
private:
    VAO *object;
};

// #endif // Plane_H
