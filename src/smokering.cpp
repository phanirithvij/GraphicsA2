#include "smokering.hpp"
#include "main.h"
#include "plane.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>
#define SCALE_RING 3

extern Plane plane;

SmokeRing::SmokeRing(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    radius = 13.0 / 2.0;


    GLfloat vertex_buffer_data[] = {}; //load the obj file of the SmokeRingv1.obj
    // GLfloat colorbuff[] = {};

    this->object = create3DObject(
        SMOKERING_vertices, SMOKERING_uvs,
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/uvmap2.DDS", GL_FILL);
        // "assets/SmokeRing.png", GL_FILL);
}

void SmokeRing::scale(double scl_){
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;

    radius *= scl_;

    glm::vec3 tempV;
    for (int i=0; i <SMOKERING_vertices.size(); i++){
        tempV.x = SMOKERING_vertices[i].x * scl_;
        tempV.y = SMOKERING_vertices[i].y * scl_;
        tempV.z = SMOKERING_vertices[i].z * scl_;
        vertices.push_back(tempV);
    }
    this->object = create3DObject(
        vertices, PLANE_uvs,
        "assets/uvmap2.DDS", GL_FILL);
}


bool SmokeRing::plane_did_it(){
    if (position.x ){
        return false;
    }
    return true;
}

bool SmokeRing::candelete(){
    if (dead) return true;
}

void SmokeRing::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of SmokeRing arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void SmokeRing::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void SmokeRing::tick(float offset) {
    // this->rotation.x += (1 * offset);
    // this->position.x -= speed;
    // this->position.y -= speed;
}

