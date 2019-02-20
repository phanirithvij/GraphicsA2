#pragma once
#include "main.h"
// #ifndef FakeHuman_H
// #define FakeHuman_H


class FakeHuman {
public:
    FakeHuman() {}
    FakeHuman(float x, float y, float z, color_t color);
    float measureY;
    float measureX;
    float measureZ;
    double speed_c;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    glm::vec3 moving;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    void move();
private:
    VAO *object;
};

// #endif // FakeHuman_H
