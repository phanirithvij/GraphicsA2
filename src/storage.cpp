#include "storage.h"
#include "main.h"
#include "objloader.hpp"
// #include <iostream>

std::vector<glm::vec3> BOMB_vertices;
std::vector<glm::vec2> BOMB_uvs;
std::vector<glm::vec3> BOMB_normals;

std::vector<glm::vec3> ISLAND_vertices;
std::vector<glm::vec2> ISLAND_uvs;
std::vector<glm::vec3> ISLAND_normals;

std::vector<glm::vec3> ARROW_vertices;
std::vector<glm::vec2> ARROW_uvs;
std::vector<glm::vec3> ARROW_normals;

std::vector<glm::vec3> PLANE_vertices;
std::vector<glm::vec2> PLANE_uvs;
std::vector<glm::vec3> PLANE_normals;

std::vector<glm::vec3> MISSILE_vertices;
std::vector<glm::vec2> MISSILE_uvs;
std::vector<glm::vec3> MISSILE_normals;

std::vector<glm::vec3> VOLCANO_vertices;
std::vector<glm::vec2> VOLCANO_uvs;
std::vector<glm::vec3> VOLCANO_normals;

std::vector<glm::vec3> PARACHUTE_vertices;
std::vector<glm::vec2> PARACHUTE_uvs;
std::vector<glm::vec3> PARACHUTE_normals;

std::vector<glm::vec3> HEALTH_vertices;
std::vector<glm::vec2> HEALTH_uvs;
std::vector<glm::vec3> HEALTH_normals;

std::vector<glm::vec3> FUELUP_vertices;
std::vector<glm::vec2> FUELUP_uvs;
std::vector<glm::vec3> FUELUP_normals;

std::vector<glm::vec3> CANNON_vertices;
std::vector<glm::vec2> CANNON_uvs;
std::vector<glm::vec3> CANNON_normals;

std::vector<glm::vec3> CANNONWH_vertices;
std::vector<glm::vec2> CANNONWH_uvs;
std::vector<glm::vec3> CANNONWH_normals;

std::vector<glm::vec3> CANNONBALL_vertices;
std::vector<glm::vec2> CANNONBALL_uvs;
std::vector<glm::vec3> CANNONBALL_normals;

std::vector<glm::vec3> SMOKERING_vertices;
std::vector<glm::vec2> SMOKERING_uvs;
std::vector<glm::vec3> SMOKERING_normals;

void loadCacheObjects(){

    std::cout << "LOADING ..." << std::endl;

    bool res = loadOBJ("assets/bombv1.obj", BOMB_vertices, BOMB_uvs, BOMB_normals);
    if (!res) {
        std::cout << "Error opening bomb " << std::endl;
    }

    res = loadOBJ("assets/islandlatestv2.obj", ISLAND_vertices, ISLAND_uvs, ISLAND_normals);
    if (!res) {
        std::cout << "Error opening island " << std::endl;
    }

    res = loadOBJ("assets/rocketv4.obj", PLANE_vertices, PLANE_uvs, PLANE_normals);
    if (!res) {
        std::cout << "Error opening plane " << std::endl;
    }

    res = loadOBJ("assets/missilev2.obj", MISSILE_vertices, MISSILE_uvs, MISSILE_normals);
    if (!res) {
        std::cout << "Error opening missile " << std::endl;
    }

    res = loadOBJ("assets/volcanov3.obj", VOLCANO_vertices, VOLCANO_uvs, VOLCANO_normals);
    if (!res) {
        std::cout << "Error opening VOLCANO " << std::endl;
    }

    res = loadOBJ("assets/parachutev1.obj", PARACHUTE_vertices, PARACHUTE_uvs, PARACHUTE_normals);
    if (!res) {
        std::cout << "Error opening parachute " << std::endl;
    }

    res = loadOBJ("assets/cannonball.obj", CANNONBALL_vertices, CANNONBALL_uvs, CANNONBALL_normals);
    if (!res) {
        std::cout << "Error opening cannonball " << std::endl;
    }

    res = loadOBJ("assets/cannonwheels.obj", CANNONWH_vertices, CANNONWH_uvs, CANNONWH_normals);
    if (!res) {
        std::cout << "Error opening wheels " << std::endl;
    }
    
    res = loadOBJ("assets/cannon.obj", CANNON_vertices, CANNON_uvs, CANNON_normals);
    if (!res) {
        std::cout << "Error opening cannon " << std::endl;
    }

    res = loadOBJ("assets/arrowv1.obj", ARROW_vertices, ARROW_uvs, ARROW_normals);
    if (!res) {
        std::cout << "Error opening arrow " << std::endl;
    }

    res = loadOBJ("assets/fuelup.obj", FUELUP_vertices, FUELUP_uvs, FUELUP_normals);
    if (!res) {
        std::cout << "Error opening fuelup " << std::endl;
    }
    
    res = loadOBJ("assets/healthkit.obj", HEALTH_vertices, HEALTH_uvs, HEALTH_normals);
    if (!res) {
        std::cout << "Error opening health " << std::endl;
    }
    
    res = loadOBJ("assets/smokering.obj", SMOKERING_vertices, SMOKERING_uvs, SMOKERING_normals);
    if (!res) {
        std::cout << "Error opening smoke " << std::endl;
    }

    std::cout << "DONE LOADING STUFF ..." << std::endl;

}

