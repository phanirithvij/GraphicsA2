#include "main.h"
#include "timer.h"
#include "cube.h"
#include "plane.h"
#include "island.h"
#include "missile.h"
#include "arrow.h"
#include "bomb.h"
#include "volcano.h"
#include "storage.h"
#include "parachute.h"
#include "smokering.hpp"
#include "fakehuman.hpp"
#include "cannon.hpp"
#include "fuelup.hpp"
#include "healthkit.hpp"

using namespace std;

GLMatrices Matrices;
GLMatrices MatricesDash;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Plane plane;
Plane supply_plane;
Plane enemy_plane;
std::vector<Island> islands;
std::vector<Missile> missiles;
std::vector<Arrow> arrows;
std::vector<Bomb> bombs;
std::vector<Volcano> volcanos;
std::vector<Parachute> parachute_kids;
std::vector<SmokeRing> smokes;
std::vector<FuelUP> fuelups;
std::vector<HealthKit> healthkits;
// std::vector<HealthKit> c;

Bomb debug_bomb_target, debug_bomb_eye, debug_bomb_up;
FakeHuman fakhu;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float MOUSE_SPEED = 0.005f;
float horizontalAngle, verticalAngle;
float ZoomF = 4;

// float eye.x = 5*cos(camera_rotation_angle*M_PI/180.0f);
// float eye.y = 0;
// float eye.z = 5*sin(camera_rotation_angle*M_PI/180.0f);

glm::vec3 eye (5, 20, 5);
glm::vec3 target (0, 0, 0);
glm::vec3 up (0, 1, 0);

glm::vec3 eyedb (0, 0, 5);
glm::vec3 targetdb (0, 0, 0);
glm::vec3 updb (0, 1, 0);

glm::vec3 direction;
bool initial = true;

GLenum MODE_DRAW = GL_FILL;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (programID);

    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    glm::mat4 VP = Matrices.projection * Matrices.view;
    // glm::mat4 MVP;  // MVP = Projection * View * Model

    MatricesDash.view = glm::lookAt( eyedb, targetdb, updb ); //Camera for 2D
    glm::mat4 VPD = MatricesDash.projection * MatricesDash.view;
    // glm::mat4 MVPD;  // MVP = Projection * View * Model

    plane.draw(VP);
    plane.pointer.draw(VP);

    for (int i=0; i< islands.size(); i++){
        islands[i].draw(VP);
    }
    for (int i=0; i< volcanos.size(); i++){
        volcanos[i].draw(VP);
    }
    for (int i=0; i< arrows.size(); i++){
        arrows[i].draw(VP);
    }
    // for (int i=0; i< parachute_kids.size(); i++){
    //     parachute_kids[i].draw(VP);
    // }
    for (int i=0; i< missiles.size(); i++){
        if (missiles[i].candelete()){
            missiles.erase(missiles.begin() + i);
        } else {
            missiles[i].draw(VP);
        }
    }
    for (int i=0; i < bombs.size(); i++){
        if (bombs[i].candelete()){
            bombs.erase(bombs.begin() + i);
        } else {
            bombs[i].draw(VP);
        }
    }

    for (int i=0; i < parachute_kids.size(); i++){
        if (parachute_kids[i].candelete()){
            parachute_kids.erase(parachute_kids.begin() + i);
        } else {
            parachute_kids[i].draw(VP);
        }
    }
    for (int i=0; i < healthkits.size(); i++){
        if (healthkits[i].candelete()){
            healthkits.erase(healthkits.begin() + i);
        } else {
            healthkits[i].draw(VP);
        }
    }
    for (int i=0; i < smokes.size(); i++){
        if (smokes[i].candelete()){
            smokes.erase(smokes.begin() + i);
        } else {
            smokes[i].draw(VP);
        }
    }
    // std::cout << "NOPEbef " << fuelups.size() << std::endl;

    for (int i=0; i < fuelups.size(); i++){
        if (fuelups[i].candelete()){
            fuelups.erase(fuelups.begin() + i);
        } else {
            // std::cout << "jjj " << i << std::endl;
            fuelups[i].draw(VP);
        }
    }

    // std::cout << "NOPEaft " << fuelups.size() << std::endl;

    debug_bomb_target.draw(VP);
    debug_bomb_eye.draw(VP);
    debug_bomb_up.draw(VP);
    fakhu.draw(VP);

    plane.draw2D(VPD);

    supply_plane.draw(VP);
    enemy_plane.draw(VP);

}

void update_eye(){
    switch (plane.camera_state){
        case VIEW_TOWER:{
            update_eye_tower();
            break;
        }
        case VIEW_FFP:{
            update_eye_ffp();
            // std::cout << "fpp" << std::endl;
            break;
        }
        case VIEW_FFP2:{
            update_eye_ffp2();
            // std::cout << "fpp2" << std::endl;
            break;
        }
        case VIEW_UP:{
            update_eye_top();
            break;
        }
        case VIEW_HELICAM:{
            update_eye_helicam();
            break;
        }
        case VIEW_BACK:{
            update_eye_back();
            break;
        }
        default:
            break;
    }
}

void update_eye_ffp(){
    eye = plane.front_vector;
    // up = glm::vec3(0,1,0);
    up = plane.up_vector;
    // up -= eye;
    // std::cout << "UP UUP " << up.x << ", " << up.y << ", " << up.z << std::endl;
    //taking eye as the midpoint of resultant and plane.position for target
    target = eye * glm::vec3(2, 2, 2) - plane.position;
    // debug_bomb_target.position = up;
    // draw();
    // debug_bomb_eye.position = eye;
}

void update_eye_ffp2(){
    eye = plane.ffp2_eye;
    up = plane.up_vector;
    // up -= eye;
    //taking eye as the midpoint of resultant and plane.position for target
    target = plane.front_vector * glm::vec3(2, 2, 2) - plane.position;
    // debug_bomb_target.position = up;
    // debug_bomb_eye.position = eye;
}

void update_eye_helicam(){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glfwSetCursorPos(window, 1024/2, 768/2);

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (state == GLFW_PRESS || initial){
        // up = up_;
        horizontalAngle += MOUSE_SPEED * float(1024/2 - xpos );
        verticalAngle   += MOUSE_SPEED * float( 768/2 - ypos );
        initial = false;
        direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle), 
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
    }
    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f), 
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );
    // Up vector
    glm::vec3 up_ = glm::cross( right, direction );

    // glm::lookAt( plane.position, plane.position + direction, up_ );
    eye = plane.position + glm::vec3(ZoomF,ZoomF,ZoomF) * direction;
    target = plane.position;
}
void update_eye_back(){
    eye = plane.position + glm::vec3 (10, 0, 0);
    up = glm::vec3(0, 1, 0);
    target = plane.position;
}
void update_eye_top(){
    eye = plane.position + glm::vec3 (0, 30, 0);
    up = glm::vec3(1, 0, 0);
    target = plane.position;
}

void update_eye_tower(){
    eye.x = 5;
    eye.y = 50;
    eye.z = 0;
    up = glm::vec3(0, 1, 0);
    target = plane.position;
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up_ = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int W = glfwGetKey(window, GLFW_KEY_W);
    int S = glfwGetKey(window, GLFW_KEY_S);
    int A = glfwGetKey(window, GLFW_KEY_A);
    int B = glfwGetKey(window, GLFW_KEY_B);
    int D = glfwGetKey(window, GLFW_KEY_D);
    int O = glfwGetKey(window, GLFW_KEY_O);
    int P = glfwGetKey(window, GLFW_KEY_P);
    int I = glfwGetKey(window, GLFW_KEY_I);
    int J = glfwGetKey(window, GLFW_KEY_J);
    int K = glfwGetKey(window, GLFW_KEY_K);
    int L = glfwGetKey(window, GLFW_KEY_L);
    int Q = glfwGetKey(window, GLFW_KEY_Q);
    int E = glfwGetKey(window, GLFW_KEY_E);
    int V = glfwGetKey(window, GLFW_KEY_V);
    int H = glfwGetKey(window, GLFW_KEY_H);
    // int MOUSE_LEFT = glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT);
    // int MOUSE_RIGHT = glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT);
    // if (state == GLFW_PRESS)

    plane.moving.x = plane.moving.y = plane.moving.z = false; //plane not moving is set

    // plane.pointer.debug_point(I, J, K);

    if (O) MODE_DRAW = GL_FILL;
    else if (P) MODE_DRAW = GL_POINT; //rendering lines/fill/points
    else if (L) MODE_DRAW = GL_LINE;

    if (Q) plane.rotation.y ++;
    else if (E) plane.rotation.y --;

    // if(minus){
    //     plane.rotation.z --;
    // } else if (equal){
    //     plane.rotation.z ++;
    // }
    if (V){
        plane.looping = true;
        // plane.looping_done = false;
    } else if (B){
        plane.barelling = true;
        // plane.barelling_done = false;
    }

    // if (W){
    //     // plane.speed.x =  -plane.speed_c * cos(glm::radians(plane.rotation.y)) * cos(glm::radians(plane.rotation.z));
    //     // plane.speed.z =   plane.speed_c * sin(glm::radians(plane.rotation.y)) * cos(glm::radians(plane.rotation.z));
    //     // plane.speed.y =   plane.speed_c * sin(glm::radians(plane.rotation.z));
    //     // plane.position.x += plane.speed.x;
    //     // plane.position.z += plane.speed.z;
    //     // plane.position.y += plane.speed.y;
    //     // plane.moving.x = true;
    //     // plane.moving.z = true;
    // }
    // else if (S){
    //     // plane.speed.x = plane.speed_c * cos(glm::radians(plane.rotation.y));
    //     // plane.speed.z = -plane.speed_c * sin(glm::radians(plane.rotation.y));
    //     // // plane.position.x += plane.speed.x;
    //     // // plane.position.z += plane.speed.z;
    //     // // plane.moving.x = true;
    //     // // plane.moving.z = true;
    // }
    if (A) {
        plane.rotation.x ++;
    } else if (D){
        plane.rotation.x --;
    }

    if (W) plane.speed_c = 1;
    else plane.speed_c = 0.5;

    if(plane.looping) plane.speed_c = 2;

    if (H){
        plane.speed_c = 0;
    }

    if(up_){
        plane.position.y += plane.speed_c;
        plane.moving.y = true;
    } else if (down){
        plane.position.y -= plane.speed_c;
        plane.moving.y = true;
    }

    update_eye(); // UPDATE EYE

}

void tick_elements() {
    plane.tick(1);
    plane.pointer.tick(1);

    supply_plane.tick(SUPPLY_PLANE);
    enemy_plane.tick(ENEMY_PLANE);

    for (int i=0; i< missiles.size(); i++){
        missiles[i].tick(1);
    }
    for (int i=0; i< bombs.size(); i++){
        bombs[i].tick(1);
    }
    for (int i=0; i< parachute_kids.size(); i++){
        parachute_kids[i].tick(1);
    }
    for (int i=0; i< islands.size(); i++){
        islands[i].pointer.tick(1);
    }
    for (int i=0; i< fuelups.size(); i++){
        fuelups[i].tick(1);
    }
    for (int i=0; i< healthkits.size(); i++){
        healthkits[i].tick(1);
    }
    for (int i=0; i< smokes.size(); i++){
        smokes[i].tick(1);
    }
    // for (int i=0; i< healthkits.size(); i++){
        // healthkits[i].tick(1);
    // }

    for (int i=0;i < volcanos.size(); i++){ //plane volcano
        bool dechealth = volcanos[i].plane_on_my_top(); //ontop => decrement health or collided => kill plane
        if (dechealth){
            plane.HP -= 0.1;
            // std::cout << "HP " << plane.HP << ", Num volcan " << volcanos.size() << std::endl;
        }
    }


    debug_bomb_target.set_position(supply_plane.front_vector * glm::vec3(2, 2, 2) - supply_plane.position);
    // std::cout << "DEBUGGING IS MY LIFE " << debug_bomb_target.position.x << ", " << debug_bomb_target.position.y << ", " << debug_bomb_target.position.z << std::endl;
    debug_bomb_eye.set_position(supply_plane.front_vector);
    debug_bomb_up.set_position(supply_plane.up_vector);
    // debug_bomb_target.set_position(plane.pointer.points_toward);

    //missile with parachuteman

    for (int i=0; i< missiles.size(); i++){
        for (int j=0; j < parachute_kids.size(); j++){
            if (parachute_kids[j].collides(missiles[i].bbox)){
                parachute_kids[j].dead = true;
                plane.score += 10;
            }
        }
    }


    for (int i=0; i< missiles.size(); i++){
        for (int j=0; j < islands.size(); j++){
            for(int k=0; k< islands[j].cannons.size(); k++){
                if (islands[j].cannons[k].collides(missiles[i].bbox)){
                    islands[j].cannons[k].dead = true;
                }
            }
        }
    }


    for (int i=0; i< missiles.size(); i++){
        for (int j=0; j < parachute_kids.size(); j++){
            if (parachute_kids[j].collides(missiles[i].bbox)){
                parachute_kids[j].dead = true;
            }
        }
    }

}

void initGL(GLFWwindow *window, int width, int height) {

    loadCacheObjects(); //caching from obj files

    plane          = Plane(-5, 40, 0, COLOR_GREEN);
    supply_plane   = Plane(-MAX_GAME_DIMENSION, 100, -MAX_GAME_DIMENSION, COLOR_GREEN);
    supply_plane.scale(6);

    enemy_plane   = Plane(-MAX_GAME_DIMENSION, 150, -MAX_GAME_DIMENSION, COLOR_GREEN);
    enemy_plane.scale(10);


    Island temp_is = Island(0, 0, 0, COLOR_BLACK);
    islands.push_back(temp_is);

    temp_is = Island(-275, 0, 0, COLOR_BLACK);
    islands.push_back(temp_is);

    temp_is = Island(275, 0, 0, COLOR_BLACK);
    islands.push_back(temp_is);

    temp_is = Island(0, 0, 275, COLOR_BLACK);
    islands.push_back(temp_is);

    temp_is = Island(0, 0, -275, COLOR_BLACK);
    islands.push_back(temp_is);

    Volcano temp_vol = Volcano(0, 0, 0, COLOR_RED);
    volcanos.push_back(temp_vol);

    temp_vol = Volcano(190, 0, 190, COLOR_RED);
    volcanos.push_back(temp_vol);

    temp_vol = Volcano(-230, 0, -230, COLOR_RED);
    volcanos.push_back(temp_vol);

    SmokeRing smring = SmokeRing(10, 60, 10, COLOR_BACKGROUND);
    smokes.push_back(smring);

    smring = SmokeRing(-210, 250, 100, COLOR_BACKGROUND);
    smokes.push_back(smring);
    smring = SmokeRing(-210, 210, -110, COLOR_BACKGROUND);
    smokes.push_back(smring);
    smring = SmokeRing(-10, 260, -110, COLOR_BACKGROUND);
    smokes.push_back(smring);
    smring = SmokeRing(120, 220, 210, COLOR_BACKGROUND);
    smokes.push_back(smring);
    smring = SmokeRing(11, 260, 110, COLOR_BACKGROUND);
    smokes.push_back(smring);
    smring = SmokeRing(120, 220, 110, COLOR_BACKGROUND);
    smokes.push_back(smring);


    Parachute para = Parachute(0, 0, 0, COLOR_BLUE);
    parachute_kids.push_back(para);
    para = Parachute(100, 590, 110, COLOR_BLACK);
    parachute_kids.push_back(para);
    para = Parachute(8, 90, 122, COLOR_BLACK);
    parachute_kids.push_back(para);
    para = Parachute(61, 101, 141, COLOR_BLACK);
    parachute_kids.push_back(para);
    para = Parachute(42, 130, 162, COLOR_BLACK);
    parachute_kids.push_back(para);
    para = Parachute(22, 80, 218, COLOR_BLACK);
    parachute_kids.push_back(para);

    FuelUP fule = FuelUP(0, 0, 0, COLOR_BLUE);
    fuelups.push_back(fule);
    fule = FuelUP(102, 90, 110, COLOR_BLACK);
    fuelups.push_back(fule);
    fule = FuelUP(18, 100, 212, COLOR_BLACK);
    fuelups.push_back(fule);
    fule = FuelUP(62, 60, 124, COLOR_BLACK);
    fuelups.push_back(fule);
    fule = FuelUP(14, 190, 26, COLOR_BLACK);
    fuelups.push_back(fule);
    fule = FuelUP(22, 290, 18, COLOR_BLACK);
    fuelups.push_back(fule);

    fakhu = FakeHuman(0, -0, 0, COLOR_BLUE);

    debug_bomb_target = Bomb(plane.position.x, plane.position.y, plane.position.z, COLOR_GREEN);
    debug_bomb_eye = Bomb(plane.position.x, plane.position.y, plane.position.z, COLOR_GREEN);
    debug_bomb_up = Bomb(plane.position.x, plane.position.y, plane.position.z, COLOR_GREEN);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    MatricesDash.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    //disable mouse view
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            // Swap Frame Buffer in double buffering
            draw();
            glfwSwapBuffers(window);

            tick_input(window);
            tick_elements();
            char text[100];
            sprintf(text, "Fuel = %d , HP = %0.1f, SCORE = %d, SPEED = %0.2f, ALTITUDE = %.2f", plane.fuel, plane.HP,
            plane.score, plane.speed_c, plane.position.y);
            glfwSetWindowTitle(window, text);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return ( abs(a.x - b.x) * 2 < (a.length + b.length)) &&
           ( abs(a.y - b.y) * 2 < (a.height + b.height)) &&
           ( abs(a.z - b.z) * 2 < (a.width  + b.width) );
}

// void reset_screen() {
//     float top    = screen_center_y + 4 / screen_zoom;
//     float bottom = screen_center_y - 4 / screen_zoom;
//     float left   = screen_center_x - 4 / screen_zoom;
//     float right  = screen_center_x + 4 / screen_zoom;
//     MatricesDash.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
// }
