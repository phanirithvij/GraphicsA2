#include "parachute.h"
#include "main.h"
#include "plane.h"
// #include "myutils.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern std::vector<glm::vec3> PARACHUTE_vertices;
extern std::vector<glm::vec2> PARACHUTE_uvs;
extern std::vector<glm::vec3> PARACHUTE_normals;

extern Plane plane;

Parachute::Parachute(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    speed_c = .01;
    speed.x = 0;
    speed.y = speed_c;
    speed.z = 0;
    //  = VIEW_BACK;
    start_time = getEpochTime();

    depthtop     = 4.8;
    depthbottom  = 0.4;
    heightbottom = 6.2;
    heighttop    = 1.72;
    lengthbottom = 0.74;
    lengthtop    = 4.8;

    bbox_top.height = heighttop * SCALE_PARACHUTE;
    bbox_top.width = depthtop * SCALE_PARACHUTE;
    bbox_top.length = lengthtop * SCALE_PARACHUTE;

    bbox_bottom.height = heightbottom * SCALE_PARACHUTE;
    bbox_bottom.width = depthbottom * SCALE_PARACHUTE;
    bbox_bottom.length = lengthbottom * SCALE_PARACHUTE;

    angularv = 0.1;

    std::vector<glm::vec3> vertices;

    glm::vec3 tempV;
    for (int i=0; i <PARACHUTE_vertices.size(); i++){
        tempV.x = PARACHUTE_vertices[i].x * SCALE_PARACHUTE;
        tempV.y = PARACHUTE_vertices[i].y * SCALE_PARACHUTE;
        tempV.z = PARACHUTE_vertices[i].z * SCALE_PARACHUTE;
        vertices.push_back(tempV);
    }

    // std::cout << "UV SIZE " << PARACHUTE_uvs.size() << " NUMVERTCES " << PARACHUTE_vertices.size() << std::endl;
    // std::cout << "RES BOOL " << "res" << std::endl;
    
    this->object = create3DObject(
        PARACHUTE_vertices, PARACHUTE_uvs,
        "assets/uvmap2.DDS", GL_FILL);

}

void Parachute::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Parachute arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Parachute::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

bool Parachute::candelete(){
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

bool Parachute::collides(bounding_box_t mbox){
    if (
        detect_collision(bbox_bottom, mbox) ||
        detect_collision(bbox_top, mbox)
    ) {
        return true;
    }
    return false;
}

bool Parachute::iscolliding(){

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

void Parachute::tick(float offset) {
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
    bbox_top.y = position.y + 2.54 * SCALE_PARACHUTE; //center to center distance
    bbox_top.z = position.z;

    dead = iscolliding();
    if (dead){
        std::cout << "DEAD" << std::endl;
        plane.score += 10;
        plane.HP -= 2;
        plane.murder_count ++;
    }
    // plane.position 
    // rotation.y -= 1;
}
