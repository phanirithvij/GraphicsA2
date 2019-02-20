#include "plane.h"
#include "main.h"
#include "fuelup.hpp"
#include "parachute.h"
#include "healthkit.hpp"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

extern std::vector<Parachute> parachute_kids;
extern std::vector<FuelUP> fuelups;
extern std::vector<HealthKit> healthkits;

extern Plane plane;

struct VAO *create2DObject(
    const GLfloat vertex_buffer_data[],
    color_t color,
    const int numverts);

struct VAO *create2DObject(GLfloat vertex_buffer_data[], const color_t color, int numvertices) {
    return create3DObject(vertex_buffer_data, numvertices,
                    color.r / 256.0, color.g / 256.0, color.b / 256.0);
}

Plane::Plane(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->rotation.z = 0;
    fuel  = 100;
    HP    = 100;
    score = 0;
    moving.x = moving.y = moving.z = false;
    camera_state = VIEW_BACK;
    speed_c = 0.2;
    speed.x = speed.y = speed.z = speed_c;

    scale_  = 1;

    length = 3.0; // 2,3,4 // z
    height = 2.0; // 2, 3 // y
    depth = 14.0; // x
    lengthwing = 13.0;
    depthwing = 1.3; // 2.1-0.3
    heightwing = 0.3;

    murder_count = 0;

    bbox_main.width = height;
    bbox_main.height = length;
    bbox_main.length = depth;

    bbox_wings.width = lengthwing;
    bbox_wings.height = heightwing;
    bbox_wings.length = depthwing;

    looping = barelling = dead = false;
    // looping_done = barelling_done = false;

    angularv = 2;

    pointer = Arrow(x, y, z, COLOR_RED);
    pointer.scale(0.3);

    GLfloat vertex_buffer_data[] = {
        0, 0, 0,
        1, 1, 0,
        1, -1, 0,
    };
    GLfloat vertex_buffer_compass[] = {
         0,   1, 0,
        -0.1, 0, 0,
         0.1, 0, 0,

         0,  -1, 0,
        -0.1, 0, 0,
         0.1, 0, 0,
    };
    //load the obj file of the planev1.obj
    randint_p = (rand() % 4 + 1);
    std::vector<glm::vec3> com_vertices;
    std::vector<glm::vec2> com_uvs;
    std::vector<glm::vec3> com_normals;

    this->object = create3DObject(
        PLANE_vertices, PLANE_uvs,
        "assets/uvmap2.DDS", GL_FILL);

    bool r = loadOBJ("assets/compass.obj", com_vertices, com_uvs, com_normals);

    scoreobj = create2DObject(vertex_buffer_data, color, 3);
    // compass  = create2DObject(vertex_buffer_compass, color, 2*3);

    for (int i=0; i < com_vertices.size(); i++){
        com_vertices[i] *= SCALE_COMPASS;
    }
    compass  = create3DObject(
        com_vertices, com_uvs,
        "assets/uvmap2.DDS", GL_FILL
        // "assets/compass.png", GL_FILL
    );
}

void Plane::scale(double scl){
    std::vector<glm::vec3> vertices;
    // std::vector<glm::vec2> uvs;
    scale_ = scl;

    bbox_main.width = height * scale_;
    bbox_main.height = length * scale_;
    bbox_main.length = depth * scale_;

    bbox_wings.width = lengthwing * scale_;
    bbox_wings.height = heightwing * scale_;
    bbox_wings.length = depthwing * scale_;

    glm::vec3 tempV;
    for (int i=0; i <PLANE_vertices.size(); i++){
        tempV.x = PLANE_vertices[i].x * scl;
        tempV.y = PLANE_vertices[i].y * scl;
        tempV.z = PLANE_vertices[i].z * scl;
        vertices.push_back(tempV);
    }
    this->object = create3DObject(
        vertices, PLANE_uvs,
        "assets/uvmap2.DDS", GL_FILL);
}

void Plane::drop_supply(supply_item_t type){
    switch (type)
    {
        case PARACHUTE_MAN:{
            Parachute drop = Parachute(position.x, position.y, position.z, COLOR_RED);
            parachute_kids.push_back(drop);
            break;
        }
        case FUEL_UP:{
            FuelUP kjhg = FuelUP(position.x, position.y, position.z, COLOR_RED);
            fuelups.push_back(kjhg);
            // std::cout << "FUEL HAS ARRIVED" << std::endl;
            break;
        }
        case HEALTH_KIT:{
            HealthKit meme = HealthKit(position.x, position.y, position.z, COLOR_RED);
            healthkits.push_back(meme);
            break;
        }
        default:{
            break;
        }
    }
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Plane arouund which we waant to rotate
    if (looping){
        // rotate          = rotate * glm::translate(glm::vec3(0, -3, 0));
    }
    if (barelling){
        rotate          = rotate * glm::translate(glm::vec3(0, -20, 0));
        translate      *= glm::translate(glm::vec3(0, 20, 0));
    }
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

bool Plane::barrel_roll(){
    // std::cout << "Started rot.x " << rotation.x << std::endl;
        
    if (barelling && rotation.x > -360){
        rotation.x -= angularv * 1.5;
        return 1;
    }
    // if(barelling_done) {
        // barelling_done = false;
    // }
    if (rotation.x <= -360){
        rotation.x = 0;
        // position.y -= 10;
        return 0;
    }
    // return 1;
    // return true;
}

bool Plane::loop_in_a_loop(){
    if (looping && rotation.z > -360){
        // std::cout << "Started looping" << std::endl;
        rotation.z -= angularv;
        return true;
    }
    if (rotation.z <= -360) {
        rotation.z = 0;
        return false;
    }

    // completed looping rightnow
}

void Plane::crash_plane(){
    if (dead){
        std::cout << "CRASH BABY" <<std::endl;
    }
}

void Plane::move(){
    position.x -= speed.x * cos(glm::radians(rotation.y));
    // position.y += speed;
    position.z -= speed.z * sin(glm::radians(rotation.y));
}

void Plane::refresh_plane(){
    randint_p = (rand() % 8 - 3);
    if (randint_p == 0){
        randint_p = 1;
    }
};

void Plane::draw2D(glm::mat4 VPD){
    MatricesDash.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (glm::vec3(12, 7, 0));    // glTranslatef
    glm::mat4 rotate;
    if (camera_state == VIEW_FFP || camera_state == VIEW_FFP2){
        rotate = glm::rotate(glm::radians(-rotation.y-90.0f), glm::vec3(0, 0, 1));
    }
    else if (camera_state == VIEW_TOWER){
        rotate = glm::rotate((float) (atan2(position.z, position.x) - (270 * M_PI / 180)), glm::vec3(0, 0, 1));
    } else{
        rotate = glm::rotate(glm::radians(0.0f - 90), glm::vec3(0, 0, 1));
    }

    rotate             *= glm::rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
    rotate             *= glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
    MatricesDash.model *= (translate * rotate);
    glm::mat4 MVP = VPD * MatricesDash.model;
    glUniformMatrix4fv(MatricesDash.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // draw3DObject(this->scoreobj);
    draw3DObject(this->compass);
    // draw3DObject(this->arrow);
    // std::cout << "TRYINA DRAW HERE NIGGA" << std::endl;
    // std::cout << scoreobj->NumVertices << std::endl;
}

void Plane::tick(float offset) {

    if (HP <= 0) dead = true;
    if (dead) {
        std::cout << "GAME OVER ++" << std::endl;
        exit(0);
    }

    speed.x =  -speed_c * cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.z));
    speed.z =   speed_c * sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.z));
    speed.y =  -speed_c * sin(glm::radians(rotation.z));
    position.x += speed.x;
    position.z += speed.z;
    position.y += speed.y;
    moving.x = true;
    moving.z = true;

    if(!loop_in_a_loop()){
        looping = false;
    }
    // int doz = ;
    if(!barrel_roll()){
        barelling = false;
        // barelling_done = true;
    }
    // if(doz==0){
    //     barelling = false;
    //     // barelling_done = true;
    // }
    

    double zh = 0.8;

    pointer.point_toward(position);
    front_vector.x = position.x - 10 * cos(glm::radians(rotation.y));
    front_vector.y = position.y /* + 10 * cos(glm::radians(rotation.y)) */;
    front_vector.z = position.z + 10 * sin(glm::radians(rotation.y));

    ffp2_eye.x = position.x + 10 * cos(glm::radians(rotation.y));
    ffp2_eye.y = position.y /* + 10 * cos(glm::radians(rotation.y)) */;
    ffp2_eye.z = position.z - 10 * sin(glm::radians(rotation.y));

    // up_vector.x = position.x /* - 10 * cos(glm::radians(rotation.y)) */;
    // up_vector.y = position.y + 10 * cos(glm::radians(rotation.x));
    // up_vector.z = position.z + 10 * sin(glm::radians(rotation.x));

    side_vector.x = position.x - 10 * sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    side_vector.y = position.y + 10 * sin(glm::radians(rotation.x));
    side_vector.z = position.z - 10 * cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

    up_vector = glm::cross(side_vector-position, front_vector-position);
    up_vector = glm::normalize(up_vector);
    up_vector *= 10;
    // up_vector += position;

    ffp2_eye = ffp2_eye +  glm::vec3( zh, zh, zh ) * up_vector;

    pointer.position = side_vector;

    if (offset == SUPPLY_PLANE || offset == ENEMY_PLANE){
        if (position.x > MAX_GAME_DIMENSION){
            position.x = -MAX_GAME_DIMENSION;
            refresh_plane();
        }
        if (position.z > MAX_GAME_DIMENSION){
            position.z = -MAX_GAME_DIMENSION;
            refresh_plane();
        }

        position.x ++;
        position.z = plane.position.z + randint_p * (position.x * position.x) / MAX_GAME_DIMENSION; // z = kx line
        
        //slope = dz/dx = 2 * pos.x * radint / MAX_G_D;

        // rotation.y = atan( (2 * position.x * randint_p) / MAX_GAME_DIMENSION );
        rotation.y = atan( - (2 * position.x * randint_p) / MAX_GAME_DIMENSION ) + 180;

        if (
            (position.x - plane.position.x < 80) &&
            (position.x - plane.position.x > -80) &&
            (fuelups.size() + healthkits.size() + parachute_kids.size() < 100) &&
            plane.murder_count < 50){
                // std::cout << "rand = " << randint_p << std::endl;
            if(offset==ENEMY_PLANE){
                if ((int)(position.x) % 11 == 1)
                drop_supply(PARACHUTE_MAN);
            }
            if (offset == SUPPLY_PLANE){
                if ((int)(position.x) % 26 == 1)
                drop_supply(FUEL_UP);
                if ((int)(position.x) % 45 == 1)
                drop_supply(HEALTH_KIT);
            }
        }
        // std::cout << "X = " << position.x << ", Z = " << position.z << std::endl;
    }

    bbox_main.x = position.x;
    bbox_main.y = position.y;
    bbox_main.z = position.z;

    bbox_wings.x = position.x - 2.6 * scale_;
    bbox_wings.y = position.y;
    bbox_wings.z = position.z;

    // std::cout << "UPUP WE GO " << up_vector.x << ", " << up_vector.y << ", " << up_vector.z << std::endl;
}
