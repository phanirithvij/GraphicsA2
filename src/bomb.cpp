#include "bomb.h"
#include "main.h"
// #include "myutils.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>
#include "plane.h"

// double GRAVITY_CONST = 0.00003;

extern Plane plane;

extern std::vector<glm::vec3> BOMB_vertices;
extern std::vector<glm::vec2> BOMB_uvs;
extern std::vector<glm::vec3> BOMB_normals;


Bomb::Bomb(float x, float y, float z, color_t color) {
    position = glm::vec3(x, y, z);
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    speed_c = 0.03;
    dead = false;
    std::cout << "Truth values X= " << plane.moving.x << ", Y= " << plane.moving.y << ", Z= " << plane.moving.z << std::endl;
    speed.x =  (plane.moving.x) ? plane.speed.x : 0;
    speed.y =  (plane.moving.y) ? plane.speed.y : speed_c;
    speed.z =  (plane.moving.z) ? plane.speed.z : 0;
    //  = VIEW_BACK;
    start_time = getEpochTime();

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the Bombv1.obj

    // std::cout << "UV SIZE " << BOMB_uvs.size() << " NUMVERTCES " << BOMB_vertices.size() << std::endl;
    // std::cout << "RES BOOL " << "res" << std::endl;

    this->object = create3DObject(
        BOMB_vertices, BOMB_uvs,
        "assets/uvmap2.DDS", GL_FILL);

}

void Bomb::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Bomb arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Bomb::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Bomb::set_position(glm::vec3 target) {
    // std::cout << "Pointing " << std::endl;
    this->position = target;
}

bool Bomb::candelete(){
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

void Bomb::tick(float offset) {
    curr_time = getEpochTime();

    int64_t t = (curr_time - start_time);
    position.y = initPosY - speed.y * t - 0.5f * GRAVITY_CONST * t * t;

    position.x += speed.x;
    position.z += speed.z;
    rotation.y -= 1;
}
