#include "healthkit.hpp"
#include "main.h"
#include "plane.h"
// #include "fuelup.hpp"
// #include "myutils.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>
#define SCALE_HEALTH 0.7

extern std::vector<glm::vec3> HEALTH_vertices;
extern std::vector<glm::vec2> HEALTH_uvs;
extern std::vector<glm::vec3> HEALTH_normals;

extern Plane plane;

HealthKit::HealthKit(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    speed_c = 0.07;
    speed.x = 0;
    speed.y = speed_c;
    speed.z = 0;
    dead = false;
    //  = VIEW_BACK;
    start_time = getEpochTime();

    heightbottom = 7;
    depthbottom = 4;
    lengthbottom = 4;

    lengthtop = 12.3;
    heighttop = 4.5;
    depthtop = 12.3;

    bbox_top.height = heighttop * SCALE_HEALTH;
    bbox_top.width = depthtop * SCALE_HEALTH;
    bbox_top.length = lengthtop * SCALE_HEALTH;

    bbox_bottom.height = heightbottom * SCALE_HEALTH;
    bbox_bottom.width = depthbottom * SCALE_HEALTH;
    bbox_bottom.length = lengthbottom * SCALE_HEALTH;

    angularv = 0.1;

    std::vector<glm::vec3> vertices;

    glm::vec3 tempV;
    for (int i=0; i <HEALTH_vertices.size(); i++){
        tempV.x = HEALTH_vertices[i].x * SCALE_HEALTH;
        tempV.y = HEALTH_vertices[i].y * SCALE_HEALTH;
        tempV.z = HEALTH_vertices[i].z * SCALE_HEALTH;
        vertices.push_back(tempV);
    }

    this->object = create3DObject(
        HEALTH_vertices, HEALTH_uvs,
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/uvmap2.DDS", GL_FILL);

}

bool HealthKit::iscollidingplane(){
    //logic
    if(
        detect_collision(bbox_bottom, plane.bbox_main) ||
        detect_collision(bbox_bottom, plane.bbox_wings)||
        detect_collision(bbox_top,    plane.bbox_main) ||
        detect_collision(bbox_top,    plane.bbox_wings)
    ){
        std::cout << "Detected some shit bro" << std::endl;
        return true;
    }

    return false;
}

void HealthKit::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of HealthKit arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void HealthKit::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

bool HealthKit::candelete(){
    int max_x = MAX_GAME_DIMENSION;
    int max_z = MAX_GAME_DIMENSION;
    int max_y = 0;
    if (dead) return true;
    if ((position.x < max_x && position.y < max_x && position.z < max_x) 
        && (position.x > -max_x && position.y > -max_y && position.z > -max_x)){
        return false;
    } else {
        return true;
    }
}

void HealthKit::tick(float offset) {
    // curr_time = getEpochTime();

    // int64_t t = (curr_time - start_time);
    position.y -= speed.y; //- 0.5f * GRAVITY_CONST * t * t;

    position.x += speed.x;
    position.z += speed.z;
    if (rotation.x >= 10 || rotation.x <= -10) {
        angularv = -angularv;
    }
    rotation.x += angularv;

    bbox_bottom.x = position.x;
    bbox_bottom.y = position.y;
    bbox_bottom.z = position.z;

    bbox_top.x = position.x;
    bbox_top.y = position.y + 7.42 * SCALE_HEALTH; //center to center distance
    bbox_top.z = position.z;

    dead = iscollidingplane();
    if (dead){
        plane.HP = 100;
    }
    // rotation.y -= 1;
}
