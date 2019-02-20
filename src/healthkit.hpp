#pragma once
#include "main.h"

// #ifndef HealthKit_H
// #define HealthKit_H

class HealthKit {
public:
    HealthKit() {}
    HealthKit(float x, float y, float z, color_t color);
    double speed_c;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 speed;
    int64_t start_time;
    int64_t curr_time;
    bounding_box_t bbox_top;
    bounding_box_t bbox_bottom;
    double initPosY;
    bool dead;
    float depthtop;
    float lengthtop;
    float heighttop;
    float depthbottom;
    float heightbottom;
    float lengthbottom;
    float angularv;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    bool candelete();
    bool iscollidingplane();
private:
    VAO *object;
};

// #endif // HealthKit_H
