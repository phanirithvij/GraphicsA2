#include "cannon.hpp"
#include "main.h"
#include "plane.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern Plane plane;
Cannon::Cannon(float x, float y, float z, color_t color) {
    
    position = glm::vec3(x, y, z);
    wheels_position = glm::vec3(x - 2, y -2, z);

    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    
    moving.x = moving.y = moving.z = false;
    speed_c = 0.2;

    wheelradius   =  2.48;
    lengthrim     =  5.3;
    length        = 7.745;
    cannon_radius = 2.38;// (back side), 1.36 (smaller one i.e front side)

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;

    std::vector<glm::vec3> WH_vertices;
    std::vector<glm::vec2> WH_uvs;

    glm::vec3 tempV;
    for (int i=0; i <CANNON_vertices.size(); i++){
        tempV.x = CANNON_vertices[i].x * SCALE_CANNON;
        tempV.y = CANNON_vertices[i].y * SCALE_CANNON;
        tempV.z = CANNON_vertices[i].z * SCALE_CANNON;
        vertices.push_back(tempV);
    }

    for (int i=0; i <CANNONWH_vertices.size(); i++){
        tempV.x = CANNONWH_vertices[i].x * SCALE_CANNON;
        tempV.y = CANNONWH_vertices[i].y * SCALE_CANNON;
        tempV.z = CANNONWH_vertices[i].z * SCALE_CANNON;
        vertices.push_back(tempV);
    }

    body = create3DObject(
        vertices, CANNON_uvs,
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/canonlatest.png", GL_FILL);
    wheels = create3DObject(
        WH_vertices, CANNONWH_uvs,
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/cannonwheels.png", GL_FILL);
}

void Cannon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Cannon arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    draw3DObject(body);
    draw_wheels(VP);
}

void Cannon::draw_wheels(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (wheels_position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Cannon arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(wheels);
}

void Cannon::set_position(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Cannon::move(){
    position.x -= speed.x * cos(glm::radians(rotation.y));
    // position.y += speed;
    position.z -= speed.z * sin(glm::radians(rotation.y));
}

void Cannon::point_toward(glm::vec3 direc){
    //cannon moves only in the initial plane
    //up and down
}

void Cannon::shoot(glm::vec3 target){
    CannonBall canb = CannonBall(position.x, position.y, position.z, COLOR_BLACK);
    canb.initplane = plane.position;
    cannonballs.push_back(canb);
}

void Cannon::tick(float offset) {
    // pointer.point_toward(position);

        // Cannon.pointer.point_toward(Cannon.position);
    // position.y -= speed;
    for (int i=0; i< cannonballs.size(); i++){
        cannonballs[i].tick(1);
    }
}


CannonBall::CannonBall(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    moving.x = moving.y = moving.z = false;
    speed_c = 0.2;

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the Cannonv1.obj

    object = create3DObject(
        CANNONBALL_vertices, CANNONBALL_uvs,
        // "assets/uvmap2.DDS", GL_FILL);
        "assets/cannonwheels.png", GL_FILL);
}

void CannonBall::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Cannon arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(object);
}

void CannonBall::set_position(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}



void CannonBall::move(){
    position.x -= speed.x * cos(glm::radians(rotation.y));
    // position.y += speed;
    position.z -= speed.z * sin(glm::radians(rotation.y));
}

void CannonBall::tick(float offset) {

    position += (initplane - position) * glm::vec3(0.2, 0.2, 0.2);
    // pointer.point_toward(position);

    // Cannon.pointer.point_toward(Cannon.position);
}
