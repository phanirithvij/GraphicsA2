#pragma once
#include "main.h"
#include "cannon.hpp"
#include "arrow.h"
// #ifndef Island_H
// #define Island_H


class Island {
public:
    Island() {}
    Island(float x, float y, float z, color_t color);
    Arrow pointer;
    std::vector<glm::vec3> heightmap;
    std::vector<Cannon> cannons;
    glm::vec3 position;
    glm::vec3 rotation;
    double speed;
    float getHeightXZ(glm::vec3 worldcoords);
    void create_inhabitants();
    void draw(glm::mat4 VP);
    void tick(float offset);
    void set_position(float x, float y, float z);
private:
    VAO *object;
    VAO *water;
};

// #endif // Island_H
