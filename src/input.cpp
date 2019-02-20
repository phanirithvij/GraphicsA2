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

#include "main.h"
#include "plane.h"
#include "island.h"
#include "missile.h"
#include "arrow.h"
#include "bomb.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;

extern glm::vec3 eye;
extern glm::vec3 target;
extern glm::vec3 up;

extern Plane plane;
extern std::vector<Missile> missiles;
extern std::vector<Arrow> arrows;
extern std::vector<Island> islands;
extern std::vector<Bomb> bombs;

extern float camera_rotation_angle;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_1:{
                plane.camera_state = VIEW_TOWER;
                break;
            }
            case GLFW_KEY_2:{
                plane.camera_state = VIEW_BACK;
                break;
            }
            case GLFW_KEY_3:
                plane.camera_state = VIEW_HELICAM;
                break;
            case GLFW_KEY_4:
                plane.camera_state = VIEW_UP;
                break;
            case GLFW_KEY_5:
                plane.camera_state = VIEW_FFP;
                break;
            case GLFW_KEY_6:
                plane.camera_state = VIEW_FFP2;
                break;
            case GLFW_KEY_C:{
                // new Arrow
                Arrow temp_is = Arrow(plane.position.x-0, plane.position.y-10, plane.position.z+0, COLOR_GREEN);
                arrows.push_back(temp_is);
                break;
            }
            case GLFW_KEY_I:{
                // // new island
                // Island temp_is = Island(plane.position.x-20, plane.position.y-10, plane.position.z+0, COLOR_GREEN);
                // islands.push_back(temp_is);
                break;
            }
            case GLFW_KEY_X:
                // // rectangle_rot_status = !rectangle_rot_status;
                // plane.camera_state = VIEW_TOWER;
                // eye.x = -10;
                // eye.y = 5;
                // eye.z = 0;
                // target.x = -5;
                // target.y = 0;
                // target.z = 0;
                break;
            case GLFW_KEY_P:{
                // triangle_rot_status = !triangle_rot_status;
                // plane.camera_state = VIEW_FFP;
                // eye.x = plane.position.x - 10;
                // eye.y = plane.position.y - 10;
                // eye.z = plane.position.z - 10;
                // target = plane.position;
                // // target.x = plane.position.x;
                // target.y = plane.position.y;
                // target.z = plane.position.z;
                break;
            }
            default:
                break;
        }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
    switch (key) {
    case 'Q':
    case 'q':
        // quit(window);
        break;
    default:
        break;
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {
            // return;
        } else if (action == GLFW_RELEASE) {
            glm::vec3 target;
            // target = plane.front_vector;
            target = eye;
            Missile temp_is = Missile(
                plane.position.x-0,
                plane.position.y+1*cos(glm::radians(plane.rotation.x)),
                plane.position.z+1*sin(glm::radians(plane.rotation.x)),
                target, COLOR_GREEN);
            missiles.push_back(temp_is);
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        if (action == GLFW_RELEASE) {
            Bomb temp_is = Bomb(plane.position.x-0, plane.position.y-3, plane.position.z+0, COLOR_GREEN);
            bombs.push_back(temp_is);
        }
    break;
    default:
        break;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (plane.camera_state == VIEW_HELICAM){
        extern float ZoomF;
        extern glm::vec3 direction;
        if (yoffset > 0) {
            //zoom in
            ZoomF --;
        } else if (yoffset < 0) {
            //zoom out 
            ZoomF ++;
        }
        eye = plane.position + glm::vec3(ZoomF,ZoomF,ZoomF) * direction;
        target = plane.position;
        // eye = (eye - target) * glm::vec3(ZoomF, ZoomF, ZoomF) + target;
    }
}
