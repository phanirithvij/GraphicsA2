#include "fakehuman.hpp"
#include "main.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

FakeHuman::FakeHuman(float x, float y, float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    moving.x = moving.y = moving.z = false;
    speed_c = 0.2;

    measureX = 0;
    measureY = 2.735;
    measureZ = 0;

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the FakeHumanv1.obj
    // GLfloat colorbuff[] = {};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    // bool res = loadOBJ("assets/FakeHumanv1.obj", vertices, uvs, normals);
    bool res = loadOBJ("assets/fakehuman.obj", vertices, uvs, normals);

    // std::cout << "UV SIZE " << uvs.size() << " NUMVERTCES " << vertices.size() << std::endl;
    // std::cout << "RES BOOL " << res << std::endl;

    this->object = create3DObject(
        vertices, uvs,
        // "assets/uvmap2.DDS", GL_FILL);
        "assets/uvmap2.DDS", GL_FILL);
}

void FakeHuman::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of FakeHuman arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void FakeHuman::set_position(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void FakeHuman::move()
{
    position.x -= speed.x * cos(glm::radians(rotation.y));
    // position.y += speed;
    position.z -= speed.z * sin(glm::radians(rotation.y));
}

void FakeHuman::tick(float offset)
{
    // pointer.point_toward(position);

    // FakeHuman.pointer.point_toward(FakeHuman.position);

    // std::cout << "ALT " << position.y << std::endl;
    // this->rotation.x += (speed * offset);
    // this->position.x -= speed;
    // this->position.y -= speed;
}
