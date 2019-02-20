#include "volcano.h"
#include "main.h"
#include "plane.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern Plane plane;

Volcano::Volcano(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;

    int num_triangles = 2;

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the Volcanov1.obj
    // GLfloat colorbuff[] = {};
    std::vector<glm::vec3> vertices;
    // std::vector<glm::vec2> uvs;
    // std::vector<glm::vec3> normals;

    radius = 2.4 / 2; //7.5, 8.7 ellipse diameter bottom and top diameter is 2.4
    height = 3.6;

    // bool res = loadOBJ("assets/volcanov3.obj", vertices, uvs, normals);

    glm::vec3 tempV;
    for (int i=0; i <VOLCANO_vertices.size(); i++){
        tempV.x = VOLCANO_vertices[i].x * SCALE_VOLCANO;
        tempV.y = VOLCANO_vertices[i].y * SCALE_VOLCANO;
        tempV.z = VOLCANO_vertices[i].z * SCALE_VOLCANO;
        vertices.push_back(tempV);
    }

    this->object = create3DObject(
        vertices, VOLCANO_uvs,
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/uvmap2.DDS", GL_FILL);
        // "assets/Volcano.png", GL_FILL);
}

void Volcano::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Volcano arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Volcano::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Volcano::tick(float offset) {
    // this->rotation.x += (1 * offset);
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bool Volcano::plane_on_my_top(){
    if (
        abs(plane.position.x - position.x) < radius * SCALE_VOLCANO &&
        abs(plane.position.z - position.z) < radius * SCALE_VOLCANO &&
        (plane.position.y - position.y) < (height + 0.2) * SCALE_VOLCANO
    ) {
        return true;
    }
    return false;
}
