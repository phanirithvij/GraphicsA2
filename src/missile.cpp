#include "missile.h"
#include "main.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern std::vector<glm::vec3> MISSILE_vertices;
extern std::vector<glm::vec2> MISSILE_uvs;
extern std::vector<glm::vec3> MISSILE_normals;

Missile::Missile(float x, float y, float z, glm::vec3 target_, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    //  = VIEW_BACK;
    speed_c = 0.003;
    dead = false;

    this->target = target_;

    // GLfloat vertex_buffer_data[] = {};

    bbox.length = 14.1 * SCALE_MISSILE;
    bbox.width = 3 * SCALE_MISSILE;
    bbox.height = 3 * SCALE_MISSILE;

    // bbox.x 

    std::vector<glm::vec3> vertices;
    // std::vector<glm::vec2> uvs;
    // std::vector<glm::vec3> normals;
    // bool res = loadOBJ("assets/Missilev1.obj", vertices, uvs, normals);
    // bool res = loadOBJ("assets/missilev2.obj", vertices, uvs, normals);

    glm::vec3 tempV;
    for (int i=0; i <MISSILE_vertices.size(); i++){
        tempV.x = MISSILE_vertices[i].x * SCALE_MISSILE;
        tempV.y = MISSILE_vertices[i].y * SCALE_MISSILE;
        tempV.z = MISSILE_vertices[i].z * SCALE_MISSILE;
        vertices.push_back(tempV);
    }

    this->object = create3DObject(
        vertices, MISSILE_uvs,
        "assets/uvmap2.DDS", GL_FILL);
}

void Missile::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Missile arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bool Missile::candelete(){
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

void Missile::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}


void Missile::tick(float offset) {
    position.x -= (speed_c) * target.x;
    position.z -= (speed_c) * target.z;
    position.y += (speed_c) * target.y;

    bbox.x = position.x;
    bbox.y = position.y;
    bbox.z = position.z;
    // position.x -= (speed_c); * plane.;
    // rotation.x -= 1;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

