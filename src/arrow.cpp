#include "arrow.h"
#include "main.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>
// #include "SDL2/std_imag"

Arrow::Arrow(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = -90;
    this->rotation.y = 0;
    this->rotation.z = 0;
    //  = VIEW_BACK;
    speed = 1;

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the Arrowv1.obj
    // GLfloat colorbuff[] = {};
    // std::vector<glm::vec3> vertices;
    // std::vector<glm::vec2> uvs;
    // std::vector<glm::vec3> normals;
    // bool res = loadOBJ("assets/Arrowv1.obj", vertices, uvs, normals);
    // bool res = loadOBJ("assets/arrowv1.obj", vertices, uvs, normals);

    // std::cout << "UV SIZE " << uvs.size() << " NUMVERTCES " << vertices.size() << std::endl;
    // std::cout << "RES BOOL " << res << std::endl;

    this->object = create3DObject(
        ARROW_vertices, ARROW_uvs,
        "assets/uvmap2.DDS", GL_FILL);
}

void Arrow::scale(float scl){
    std::vector<glm::vec3> vertices;
    glm::vec3 tempV;
    for (int i=0; i <ARROW_vertices.size(); i++){
        tempV.x = ARROW_vertices[i].x * scl;
        tempV.y = ARROW_vertices[i].y * scl;
        tempV.z = ARROW_vertices[i].z * scl;
        vertices.push_back(tempV);
    }
    this->object = create3DObject(
        vertices, ARROW_uvs,
        "assets/uvmap2.DDS", GL_FILL);
}

void Arrow::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Arrow arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Arrow::debug_point(int I, int J, int K){
    if (J) {
        rotation.y ++;
    }
    if (K) {
        rotation.z ++;
    }
    if (I) {
        rotation.x ++;
    }
}

void Arrow::point_toward(glm::vec3 targetpos){
    // Y up
    // X out
    // Z left
    //point towards target
    glm::vec3 vect_point;
    // vect_point = targetpos - position;
    vect_point.x = (targetpos.x - position.x);
    vect_point.y = (targetpos.y - position.y);
    vect_point.z = (targetpos.z - position.z);
    double denome = sqrt(vect_point.x * vect_point.x + /* vect_point.y * vect_point.y + */ vect_point.z * vect_point.z);

    vect_point.x = vect_point.x / denome;
    // vect_point.y = vect_point.y / denome;
    vect_point.z = vect_point.z / denome;

    // std::cout << "Double " << denome << std::endl;
    // rotation.x = acos(vect_point.x) * 180.0 / M_PI;
    rotation.y = -acos(vect_point.z) * 180.0 / M_PI;
    // rotation.z = acos(vect_point.z) * 180.0 / M_PI;
    // std::cout << "Angle " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;
    // atan2();
    points_toward = vect_point;
}

void Arrow::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}


void Arrow::tick(float offset) {
    // this->rotation.x += (speed * offset);
    // this->position.x += 0.01;
    // this->position.z += 0.1;
    // this->position.y -= speed;
}

