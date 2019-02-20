#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <iostream>
#include <string.h>
// #include "stb_image.h"

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

class Texture{
public:
    Texture(){};
    ~Texture();
    Texture(const std::string& path);
    void Bind(int slot);
    void Unbind();
    int width, height, bpp;
    std::string path;
    GLuint TextureID;
    unsigned char* local_buffer;
private:
};

#endif