#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "myutils.h"
#include "texture.hpp"

#define GRAVITY_CONST 0.000003
#define MAX_GAME_DIMENSION 600
#define SCALE_ISLAND 10
#define SCALE_PARACHUTE 0.6
#define SCALE_MISSILE 0.5
#define SCALE_CANNON 5
#define SCALE_COMPASS 0.4
#define SUPPLY_PLANE 9999
#define ENEMY_PLANE  8888
#define SCALE_VOLCANO 30

struct color_t {
    int r;
    int g;
    int b;
};

void loadCacheObjects();

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create2DObject(
    const GLfloat vertex_buffer_data[],
    color_t color,
    const int numverts);
struct VAO *create3DObject(
    const GLfloat vertex_buffer_data[],
    const GLfloat *color_buffer_data,
    const int numverts);
struct VAO *create3DObject(
    const std::vector<glm::vec3> vertices,
    const GLfloat *color_buffer_data,
    const std::vector<glm::vec2> uv_buffer_data,
    GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(
    GLfloat vertex_buffer_data[],
    int numvertices,
    const GLfloat red,
    const GLfloat green,
    const GLfloat blue);
struct VAO *create3DObject(
    const std::vector<glm::vec3> vertices,
    const std::vector<glm::vec2> uv_buffer_data,
    const GLfloat red,
    const GLfloat green,
    const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(
    const std::vector<glm::vec3> vertices,
    const std::vector<glm::vec2> uv_buffer_data,
    const color_t color,
    GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(
    const std::vector<glm::vec3> vertices,
    const std::vector<glm::vec2> uv_buffer_data,
    const std::string texture_path,
    const GLenum fill_mode);
// struct VAO *create3DObject(GLenum primitive_mode, int numVertices, int uvnums, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, const std::vector<glm::vec2> uv_buffer_data, GLenum fill_mode = GL_FILL);
// struct VAO *create3DObject(GLenum primitive_mode, int numVertices, int uvnums, const GLfloat *vertex_buffer_data, const std::vector<glm::vec2> uv_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
// struct VAO *create3DObject(GLenum primitive_mode, int numVertices, int uvnums, const GLfloat *vertex_buffer_data, const std::vector<glm::vec2> uv_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);

void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;
    GLuint UVBuffer;
    GLuint Texture_;
    GLuint TextureID;
    Texture texture;
    int slotno;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
    int    NumUVS;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

enum supply_item_t {
    PARACHUTE_MAN,
    FUEL_UP,
    HEALTH_KIT
};

extern GLMatrices Matrices;
extern GLMatrices MatricesDash;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float z;
    float width;
    float height;
    float length;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);

extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();

void update_eye();
void update_eye_ffp();
void update_eye_ffp2();
void update_eye_tower();
void update_eye_top();
void update_eye_helicam();
void update_eye_back();
// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_BLUE; // = { 0, 0, 220 };
// enums

enum state_cam_t {
    VIEW_UP,
    VIEW_BACK,
    VIEW_FFP,
    VIEW_TOWER,
    VIEW_HELICAM,
    VIEW_FFP2
};

//vertices
extern std::vector<glm::vec3> BOMB_vertices;
extern std::vector<glm::vec2> BOMB_uvs;
extern std::vector<glm::vec3> BOMB_normals;

extern std::vector<glm::vec3> ISLAND_vertices;
extern std::vector<glm::vec2> ISLAND_uvs;
extern std::vector<glm::vec3> ISLAND_normals;

extern std::vector<glm::vec3> PLANE_vertices;
extern std::vector<glm::vec2> PLANE_uvs;
extern std::vector<glm::vec3> PLANE_normals;

extern std::vector<glm::vec3> MISSILE_vertices;
extern std::vector<glm::vec2> MISSILE_uvs;
extern std::vector<glm::vec3> MISSILE_normals;

extern std::vector<glm::vec3> VOLCANO_vertices;
extern std::vector<glm::vec2> VOLCANO_uvs;
extern std::vector<glm::vec3> VOLCANO_normals;

extern std::vector<glm::vec3> PARACHUTE_vertices;
extern std::vector<glm::vec2> PARACHUTE_uvs;
extern std::vector<glm::vec3> PARACHUTE_normals;

extern std::vector<glm::vec3> CANNON_vertices;
extern std::vector<glm::vec2> CANNON_uvs;
extern std::vector<glm::vec3> CANNON_normals;

extern std::vector<glm::vec3> CANNONWH_vertices;
extern std::vector<glm::vec2> CANNONWH_uvs;
extern std::vector<glm::vec3> CANNONWH_normals;

extern std::vector<glm::vec3> CANNONBALL_vertices;
extern std::vector<glm::vec2> CANNONBALL_uvs;
extern std::vector<glm::vec3> CANNONBALL_normals;

extern std::vector<glm::vec3> HEALTH_vertices;
extern std::vector<glm::vec2> HEALTH_uvs;
extern std::vector<glm::vec3> HEALTH_normals;

extern std::vector<glm::vec3> FUELUP_vertices;
extern std::vector<glm::vec2> FUELUP_uvs;
extern std::vector<glm::vec3> FUELUP_normals;

extern std::vector<glm::vec3> ARROW_vertices;
extern std::vector<glm::vec2> ARROW_uvs;
extern std::vector<glm::vec3> ARROW_normals;

extern std::vector<glm::vec3> SMOKERING_vertices;
extern std::vector<glm::vec2> SMOKERING_uvs;
extern std::vector<glm::vec3> SMOKERING_normals;

#endif // MAIN_H
