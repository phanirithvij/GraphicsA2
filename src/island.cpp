#include "island.h"
#include "main.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern std::vector<glm::vec3> ISLAND_vertices;
extern std::vector<glm::vec2> ISLAND_uvs;
extern std::vector<glm::vec3> ISLAND_normals;

Island::Island(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;

    pointer = Arrow(x, y + 100, z, COLOR_BLUE);
    pointer.scale(6);

    pointer.point_toward(position);

    create_inhabitants();

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the Islandv1.obj
    // GLfloat colorbuff[] = {};
    std::vector<glm::vec3> vertices;
    // std::vector<glm::vec2> uvs;
    // std::vector<glm::vec3> normals;
    // bool res = loadOBJ("assets/Islandv1.obj", vertices, uvs, normals);
    // bool res = loadOBJ("assets/islandv1.obj", vertices, uvs, normals);

    glm::vec3 tempV;
    for (int i=0; i <ISLAND_vertices.size(); i++){
        tempV.x = ISLAND_vertices[i].x * SCALE_ISLAND;
        tempV.y = ISLAND_vertices[i].y * SCALE_ISLAND;
        tempV.z = ISLAND_vertices[i].z * SCALE_ISLAND;
        vertices.push_back(tempV);
    }

    // std::cout << "UV SIZE " << ISLAND_uvs.size() << " NUMVERTCES " << ISLAND_vertices.size() << std::endl;
    // std::cout << "RES BOOL " << res << std::endl;

    this->object = create3DObject(
        vertices, ISLAND_uvs,
        // "islandlatest.png", GL_FILL);
        "assets/uvmap2.DDS", GL_FILL);
        // "assets/uvmap2.DDS", GL_FILL);
}

// https://youtu.be/6E2zjfzMs7c?t=219
float Island::getHeightXZ(glm::vec3 worldcoords){
    float terr_x = worldcoords.x - position.x;
    float terr_z = worldcoords.z - position.z;
    // 128 x 128 vertices on blender and 27.5625m x 27.5625m
    
    float SIZE = 27.5625 * SCALE_ISLAND;
    float grid_square_size = SIZE / (ISLAND_vertices.size());
    float grid_x = terr_x / grid_square_size;
    float grid_z = terr_z / grid_square_size;

    float xCoord = ((int)terr_x % (int)grid_square_size) / grid_square_size;
    float zCoord = ((int)terr_z % (int)grid_square_size) / grid_square_size;

    float answer;
    // if (xCoord <= (1-zCoord)) {
    //     answer = barryCentric(glm::vec3(0, heights[grid_x][grid_z], 0), glm::vec3(1,
    //                     heights[grid_x + 1][grid_z], 0), glm::vec3(0,
    //                     heights[grid_x][grid_z + 1], 1), glm::vec2(xCoord, zCoord));
    // } else {
    //     answer = barryCentric(glm::vec3(1, heights[grid_x + 1][grid_z], 0), glm::vec3(1,
    //                     heights[grid_x + 1][grid_z + 1], 1), glm::vec3(0,
    //                     heights[grid_x][grid_z + 1], 1), glm::vec2(xCoord, zCoord));
    // }

    return answer;
}

void Island::create_inhabitants(){

    float TERRAIN_HEIGHT = 0;

    std::vector<glm::vec3> cannonpositions;

    cannonpositions.push_back(glm::vec3(3.6, 4, 8.56)); // +- 3.6
    cannonpositions.push_back(glm::vec3(6.14, 3.06, 4.83));
    cannonpositions.push_back(glm::vec3(5.13, 3.35, 2.7)); //1.35 y
    cannonpositions.push_back(glm::vec3(0, 6.04, 3.64));

    for (int i=0; i < cannonpositions.size(); i++){
        Cannon cannon = Cannon(cannonpositions[i].x * SCALE_ISLAND, cannonpositions[i].y * SCALE_ISLAND,
         cannonpositions[i].z * SCALE_ISLAND
        , COLOR_BLACK);
        cannons.push_back(cannon);
    }

}

void Island::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Island arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    pointer.draw(VP);
    for (int i=0; i<cannons.size(); i++){
        if(cannons[i].dead){
            cannons.erase(cannons.begin() + i);
        } else
        cannons[i].draw(VP);
    }
}

void Island::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Island::tick(float offset) {
    this->rotation.x += (speed * offset);
    // this->position.x -= speed;
    // this->position.y -= speed;
}

