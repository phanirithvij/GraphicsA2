#pragma once
#include "main.h"

// #ifndef SmokeRing_H
// #define SmokeRing_H


class SmokeRing {
public:
    SmokeRing() {}
    SmokeRing(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    double radius;
    bool dead;
    void set_position(float x, float y, float z);
    void tick(float offset);
    void draw(glm::mat4 VP);
    bool candelete();
    bool plane_did_it();
    void scale(double scl_);
private:
    VAO *object;
};

// #endif // SmokeRing_H
