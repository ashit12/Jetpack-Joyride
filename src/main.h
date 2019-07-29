#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float right_limit=3.0f,left_limit=-4.0f,sky_limit=3.01f,surface_limit=-3.5f,gravity=0.15/60;
const float speed_inc = 2.0/60,mag_speed=0.02f,maj_axis=2.0f,min_axis=1.0f;
const int num_bricks=20,rows=3,num_coins=500,num_plines=50,num_flines=50,num_mag=25,num_pow=30,num_boom=15,max_life=5,immune=120,num_boss=15,num_ring=10;
extern std::vector<std::string> vec;
extern int cur_life,hold,fi;
extern float back_move_limit,magnify,curx,cury,cur_rad;
struct color_t {
    int r;
    int g;
    int b;
};


// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
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

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float width;
    float height;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);
float transy(float x,float y,double ang);
float transx(float x,float y,double ang);
float dist(float x,float y,float u,float b);


extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_WHITE;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_ARRAY[];

#endif
