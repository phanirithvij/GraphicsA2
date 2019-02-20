#pragma once
#include "main.h"

// #ifndef Bomb_H
// #define Bomb_H

class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    int64_t start_time;
    int64_t curr_time;
    double initPosY;
    double speed_c;
    bool dead;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_position(glm::vec3 target);
    void tick(float offset);
    bool candelete();

private:
    VAO *object;
};

// #endif // Bomb_H
