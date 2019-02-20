#include "fuelup.hpp"
#include "main.h"
#include "plane.h"
// #include "myutils.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>
#define SCALE_FUELUP 0.7

extern std::vector<glm::vec3> FUELUP_vertices;
extern std::vector<glm::vec2> FUELUP_uvs;
extern std::vector<glm::vec3> FUELUP_normals;

extern Plane plane;

FuelUP::FuelUP(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    speed_c = 0.05;
    speed.x = 0;
    speed.y = speed_c;
    speed.z = 0;
    dead = false;
    //  = VIEW_BACK;
    start_time = getEpochTime();

    depthtop     = 12.6;
    depthbottom  = 2.02;
    heightbottom = 6.2;
    heighttop    = 4.64;
    lengthbottom = 2.02;
    lengthtop    = 12.6;

    bbox_top.height = heighttop;
    bbox_top.width = depthtop;
    bbox_top.length = lengthtop;

    bbox_bottom.height = heightbottom;
    bbox_bottom.width = depthbottom;
    bbox_bottom.length = lengthbottom;

    angularv = 0.1;

    std::vector<glm::vec3> vertices;

    glm::vec3 tempV;
    for (int i=0; i <FUELUP_vertices.size(); i++){
        tempV.x = FUELUP_vertices[i].x * SCALE_FUELUP;
        tempV.y = FUELUP_vertices[i].y * SCALE_FUELUP;
        tempV.z = FUELUP_vertices[i].z * SCALE_FUELUP;
        vertices.push_back(tempV);
    }

    // bool res = loadOBJ("assets/parachutev1.obj", FUELUP_vertices, FUELUP_uvs, FUELUP_normals);

    // std::cout << "NEW FUEL " << FUELUP_uvs.size() << " NUMVERTCES " << FUELUP_vertices.size() << std::endl;
    // std::cout << "RES BOOL " << "res" << std::endl;

    this->object = create3DObject(
        vertices, FUELUP_uvs,
        "assets/uvmap2.DDS", GL_FILL);

}

void FuelUP::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of FuelUP arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void FuelUP::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

bool FuelUP::iscollidingPlane(){
    //logic
    if(
        detect_collision(bbox_bottom, plane.bbox_main) ||
        detect_collision(bbox_bottom, plane.bbox_wings)||
        detect_collision(bbox_top,    plane.bbox_main) ||
        detect_collision(bbox_top,    plane.bbox_wings)
    ){
        std::cout << "Detected collision" << std::endl;
        return true;
    }

    return false;
}

bool FuelUP::candelete(){
    int max_x = MAX_GAME_DIMENSION;
    int max_z = MAX_GAME_DIMENSION;
    int max_y = 0;
    if (dead) return true;
    if ((position.x < max_x && position.y < max_x && position.z < max_x) 
        && (position.x > -max_x && position.y > -max_y && position.z > -max_x)){
        return false;
    } else {
        // std::cout << "Deleted fuel" << std::endl;
        return true;
    }
}

void FuelUP::tick(float offset) {
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
    bbox_top.y = position.y + 7.54 * SCALE_FUELUP; //center to center distance
    bbox_top.z = position.z;

    dead = iscollidingPlane();
    if (dead){
        plane.fuel += 30;
        // plane
    }

    // rotation.y -= 1;
}
