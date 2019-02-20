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
#include "texture.hpp"

using namespace std;

extern GLuint programID;
extern GLenum MODE_DRAW;

int TOTAL_NOW = 0;

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow *initGLFW(int width, int height)
{
    GLFWwindow *window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        // exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        // exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // Initialize GLEW, Needed in Core profile
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error: Failed to initialise GLEW : " << glewGetErrorString(err) << endl;
        exit(1);
    }
    glfwSwapInterval(1);

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
       is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar); // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton); // mouse button clicks
    glfwSetScrollCallback(window, scroll_callback);

    return window;
}

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path)
{

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

struct VAO *create3DObject(
    const std::vector<glm::vec3> vertices,
    const std::vector<glm::vec2> uv_buffer_data,
    const std::string texture_path,
    const GLenum fill_mode)
{
    // std::cout << "HAS Texture ID" << std::endl;

    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLES;
    vao->NumVertices = vertices.size();
    vao->FillMode = fill_mode;
    vao->NumUVS = uv_buffer_data.size();

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers(1, &(vao->VertexBuffer));       // VBO - vertices
    // glGenBuffers (1, &(vao->ColorBuffer)); // VBO - colors
    glGenBuffers(1, &(vao->UVBuffer)); // VBO - uvbuffer

    glBindVertexArray(vao->VertexArrayID); // Bind the VAO

    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);                                                    // Bind the VBO vertices
    glBufferData(GL_ARRAY_BUFFER, vao->NumVertices * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
        0,        // attribute 0. Vertices
        3,        // size (x,y,z)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );
    // glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    // glBufferData (GL_ARRAY_BUFFER, 3 * vao->NumVertices * sizeof(GLfloat), {0}, GL_STATIC_DRAW); // Copy the vertex colors
    // glVertexAttribPointer(
    //     1,                            // attribute 1. Color
    //     3,                            // size (r,g,b)
    //     GL_FLOAT,                     // type
    //     GL_FALSE,                     // normalized?
    //     0,                            // stride
    //     (void *) 0                    // array buffer offset
    // );

    glBindBuffer(GL_ARRAY_BUFFER, vao->UVBuffer);                                                         // Bind the UV
    glBufferData(GL_ARRAY_BUFFER, vao->NumUVS * sizeof(glm::vec2), &(uv_buffer_data[0]), GL_STATIC_DRAW); // Copy the uv
    glVertexAttribPointer(
        1,        // attribute 2. UV
        2,        // size (1, 2)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    if (texture_path.find("DDS") != std::string::npos)
    {
        vao->TextureID = glGetUniformLocation(programID, "myTextureSampler");
        // cout << "LOADERRERERERE" << texture_path << endl;
        vao->Texture_ = loadDDS(texture_path.c_str());
    }
    else if (texture_path.find("png") != std::string::npos)
    {
        vao->texture = Texture(texture_path);
        vao->texture.TextureID = glGetUniformLocation(programID, "myTextureSampler2");
        // vao->slotno = TOTAL_NOW;
        // TOTAL_NOW++;
        // vao->texture.TextureID = vao->TextureID;
    }
    else if (texture_path.find("bmp") != std::string::npos)
    {
        // vao->Texture_ = loadBMP_custom(texture_path.c_str());
        // cout << "BPMBMPBMPBMP" << texture_path << endl;
        vao->texture = Texture(texture_path);
        vao->texture.TextureID = vao->TextureID;
    }
    // Texture = loadDDS("assets/uvmap2.DDS");
    // Get a handle for our "myTextureSampler" uniform

    return vao;
}
/* Generate VAO, VBOs and return VAO handle */
struct VAO *create3DObject(std::vector<glm::vec3> vertices,
                           const GLfloat *color_buffer_data,
                           const std::vector<glm::vec2> uv_buffer_data,
                           GLenum fill_mode)
{
    // std::cout << "No texture ID" << std::endl;
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLES;
    vao->NumVertices = vertices.size();
    vao->FillMode = fill_mode;
    vao->NumUVS = uv_buffer_data.size();
    vao->Texture_ = false;
    vao->TextureID = false;

    // cout << "UV.size() " << uvnums << endl;
    // cout << "Numvertices " << numVertices << endl;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers(1, &(vao->VertexBuffer));       // VBO - vertices
    glGenBuffers(1, &(vao->ColorBuffer));        // VBO - colors
    glGenBuffers(1, &(vao->UVBuffer));           // VBO - colors

    glBindVertexArray(vao->VertexArrayID); // Bind the VAO

    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);                                                    // Bind the VBO vertices
    glBufferData(GL_ARRAY_BUFFER, vao->NumVertices * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
        0,        // attribute 0. Vertices
        3,        // size (x,y,z)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    // glBufferData (GL_ARRAY_BUFFER, 3 * vao->NumVertices * sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW); // Copy the vertex colors
    // glVertexAttribPointer(
    //     1,                            // attribute 1. Color
    //     3,                            // size (r,g,b)
    //     GL_FLOAT,                     // type
    //     GL_FALSE,                     // normalized?
    //     0,                            // stride
    //     (void *) 0                    // array buffer offset
    // );

    glBindBuffer(GL_ARRAY_BUFFER, vao->UVBuffer);                                                         // Bind the UV
    glBufferData(GL_ARRAY_BUFFER, vao->NumUVS * sizeof(glm::vec2), &(uv_buffer_data[0]), GL_STATIC_DRAW); // Copy the uv
    glVertexAttribPointer(
        1,        // attribute 2. UV
        2,        // size (1, 2)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    return vao;
}

struct VAO *create3DObject(
    const GLfloat *vertex_buffer_data,
    const GLfloat *color_buffer_data,
    int numvertices)
{
    std::cout << "No texture ID" << std::endl;
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLES;
    vao->NumVertices = numvertices;
    vao->FillMode = GL_FILL;
    vao->NumUVS = numvertices;
    vao->Texture_ = false;
    vao->TextureID = false;

    // cout << "UV.size() " << uvnums << endl;
    // cout << "Numvertices " << numVertices << endl;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers(1, &(vao->VertexBuffer));       // VBO - vertices
    glGenBuffers(1, &(vao->ColorBuffer));        // VBO - colors
    // glGenBuffers (1, &(vao->UVBuffer)); // VBO - colors

    glBindVertexArray(vao->VertexArrayID); // Bind the VAO

    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);                                                          // Bind the VBO vertices
    glBufferData(GL_ARRAY_BUFFER, vao->NumVertices * 3 * sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
        0,        // attribute 0. Vertices
        3,        // size (x,y,z)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);                                                          // Bind the VBO colors
    glBufferData(GL_ARRAY_BUFFER, 3 * vao->NumVertices * sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW); // Copy the vertex colors
    glVertexAttribPointer(
        3,        // attribute 1. Color
        3,        // size (r,g,b)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO *create3DObject(std::vector<glm::vec3> vertices, const std::vector<glm::vec2> uv_buffer_data,
                           const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode)
{
    GLfloat *color_buffer_data = new GLfloat[3 * vertices.size()];
    for (int i = 0; i < vertices.size(); i++)
    {
        color_buffer_data[3 * i] = red;
        color_buffer_data[3 * i + 1] = green;
        color_buffer_data[3 * i + 2] = blue;
    }

    // cout << "COLOR STUFF " << uv_buffer_data.size() << endl;

    return create3DObject(vertices, color_buffer_data, uv_buffer_data, fill_mode);
}

struct VAO *create3DObject(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uv_buffer_data,
                           const color_t color, GLenum fill_mode)
{
    return create3DObject(vertices, uv_buffer_data,
                          color.r / 256.0, color.g / 256.0, color.b / 256.0, fill_mode);
}

struct VAO *create3DObject(GLfloat *vertex_buffer_data, int numvertices,
                           const GLfloat red, const GLfloat green, const GLfloat blue)
{
    GLfloat *color_buffer_data = new GLfloat[3 * numvertices];
    for (int i = 0; i < numvertices; i++)
    {
        color_buffer_data[3 * i] = red;
        color_buffer_data[3 * i + 1] = green;
        color_buffer_data[3 * i + 2] = blue;
    }

    return create3DObject(vertex_buffer_data, color_buffer_data, numvertices);
}

// struct VAO *create2DObject(GLfloat vertex_buffer_data[], const color_t color, int numvertices) {
//     return create3DObject(vertex_buffer_data, numvertices,
//                     color.r / 256.0, color.g / 256.0, color.b / 256.0);
// }

/* Render the VBOs handled by VAO */
void draw3DObject(struct VAO *vao)
{
    if (TOTAL_NOW > 30)
    {
        std::cout << "TOOMUCH BRO" << std::endl;
        exit(1);
    }
    // Change the Fill Mode for this object
    glPolygonMode(GL_FRONT_AND_BACK, MODE_DRAW /* vao->FillMode */);

    // Bind the VAO to use
    glBindVertexArray(vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // std::cout << "Nope " << vao->TextureID << std::endl;
    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    // std::cout << "Color " << sizeof(vao->ColorBuffer) << std::endl;
    // glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // glEnableVertexAttribArray(2);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->UVBuffer);
    if (vao->TextureID)
    {
        int off = 0;
        int off2 = 1;
        // std::cout << "YOP " << vao->TextureID << std::endl;
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0 + off);
        glBindTexture(GL_TEXTURE_2D, vao->Texture_);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        // glUniform1i(vao->TextureID, off);

        // vao->texture.Bind(vao->slotno);
        // std::cout << "vertices " << vao->NumVertices << std::endl;
        glUniform1i(vao->TextureID, off);
    }

    glEnableVertexAttribArray(3);

    // Draw the geometry !
    glDrawArrays(GL_TRIANGLES, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
    vao->texture.Unbind();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    // glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}
