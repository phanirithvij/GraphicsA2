#pragma once
#include "main.h"
#include "arrow.h"
// #ifndef Plane_H
// #define Plane_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y, float z, color_t color);
    Arrow pointer;
    int score;
    int fuel;
    float HP;
    double speed_c;
    state_cam_t camera_state;
    bounding_box_t bbox_main;
    bounding_box_t bbox_wings;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 moving;
    glm::vec3 speed;
    glm::vec3 front_vector;
    glm::vec3 side_vector;
    glm::vec3 up_vector;
    glm::vec3 ffp2_eye;
    float depth;
    float height;
    float length;
    float lengthwing;
    float depthwing;
    float heightwing;
    bool dead;
    bool looping;
    bool barelling;
    float angularv;
    float scale_;
    int murder_count;
    GLfloat *vertex_buffer_data_compass;
    void set_position(float x, float y, float z);
    void drop_supply(supply_item_t type);
    void draw2D(glm::mat4 VPD);
    void draw(glm::mat4 VP);
    void tick(float offset);
    void scale(double scl);
    void crash_plane();
    void move();
    void refresh_plane();
    bool barrel_roll();
    bool loop_in_a_loop();
    // bool barelling_done;
    // bool looping_done;
private:
    int randint_p;
    VAO *object;
    VAO *scoreobj;
    VAO *compass;
    VAO *arrow;
    VAO *fuelobj;
    VAO *health;
};

// #endif // Plane_H
