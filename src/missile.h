#pragma once
#include "main.h"

// #ifndef Plane_H
// #define Plane_H


class Missile {
public:
    Missile() {}
    Missile(float x, float y, float z, glm::vec3 target, color_t color);
    double speed_c;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    glm::vec3 target;
    bool dead;
    bounding_box_t bbox;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    bool candelete();
private:
    VAO *object;
};

// #endif // Plane_H
