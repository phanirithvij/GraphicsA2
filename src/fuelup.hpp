#pragma once
#include "main.h"

// #ifndef FuelUP_H
// #define FuelUP_H

class FuelUP {
public:
    FuelUP() {}
    FuelUP(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    int64_t start_time;
    int64_t curr_time;
    bounding_box_t bbox_top;
    bounding_box_t bbox_bottom;
    bool dead;
    double initPosY;
    float depthtop;
    float lengthtop;
    float heighttop;
    float depthbottom;
    float heightbottom;
    float lengthbottom;
    float angularv;
    double speed_c;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    bool candelete();
    bool iscollidingPlane();
private:
    VAO *object;
};

// #endif // FuelUP_H
