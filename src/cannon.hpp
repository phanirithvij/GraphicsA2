#pragma once
#include "main.h"
// #ifndef CANNON_H
// #define CANNON_H

class CannonBall {
public:
    CannonBall() {}
    CannonBall(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 initplane;
    glm::vec3 speed;
    glm::vec3 moving;
    double speed_c;
    bool is_flying;
    int64_t start_time;
    int64_t curr_time;
    void move();
    void set_position(float x, float y, float z);
    void tick(float offset);
    bounding_box_t bboxc;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

class Cannon {
public:
    Cannon() {}
    Cannon(float x, float y, float z, color_t color);
    std::vector<CannonBall> cannonballs;
    glm::vec3 position;
    glm::vec3 wheels_position;
    glm::vec3 rotation;
    glm::vec3 moving;
    glm::vec3 speed;
    double speed_c;
    bool dead;
    float wheelradius; // 2.48
    float lengthrim; // 5.3
    float length; //7.745
    float cannon_radius; //2.38 (back side), 1.36 (smaller one i.e front side)
    bounding_box_t bboxc;
    bool collides(bounding_box_t mbox);
    void set_position(float x, float y, float z);
    void point_toward(glm::vec3 direc);
    void draw_wheels(glm::mat4 VP);
    void tick(float offset);
    void draw(glm::mat4 VP);
    void shoot(glm::vec3 target);
    void move();
private:
    VAO *wheels;
    VAO *body;
};

// #endif // CANNON_H
