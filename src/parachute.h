#pragma once
#include "main.h"

// #ifndef Parachute_H
// #define Parachute_H

class Parachute {
public:
    Parachute() {}
    Parachute(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    int64_t start_time;
    int64_t curr_time;
    bounding_box_t bbox_top;
    bounding_box_t bbox_bottom;
    double initPosY;
    double speed_c;
    bool dead;
    double angularv;
    float depthtop;
    float lengthtop;
    float heighttop;
    float depthbottom;
    float heightbottom;
    float lengthbottom;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    bool candelete();
    bool iscolliding();
    bool collides(bounding_box_t mbox);
private:
    VAO *object;
};

// #endif // Parachute_H
