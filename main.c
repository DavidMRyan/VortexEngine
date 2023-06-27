#define GLEW_STATIC
#define VERSION "Version 0.1.3"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FOV 90.0f
#define ZNEAR 1.0f
#define ZFAR 10.0f

// Temp -> Mesh Rotation Delta
#define ROTATION_DELTA 0.005f

#include <stdio.h>
#include <math.h>
#include <process.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "src/include/util.h"
#include "src/include/math_3d.h"
#include "src/include/world_transform.h"
#include "src/include/camera.h"
#include "src/include/projection.h"

// Global Uniforms
GLuint VBO;
GLuint IBO;
GLint U_LOCALPOS;

// Transforms & Structures
// TODO: Eventually move these out of the global scope.
WorldTransform CUBE_WTRANSFORM;
Camera CAMERA_MAIN;
PersProjection PERSPECTIVE;

// -------------------------------------------
// Mesh Setup

static void create_vertex_buffer() {
    Vertex vertices[8];

    // Cube
    vertices[0] = new_vertex(0.5f, 0.5f, 0.5f);
    vertices[1] = new_vertex(-0.5f, 0.5f, -0.5f);
    vertices[2] = new_vertex(-0.5f, 0.5f, 0.5f);
    vertices[3] = new_vertex(0.5f, -0.5f, -0.5f);
    vertices[4] = new_vertex(-0.5f, -0.5f, -0.5f);
    vertices[5] = new_vertex(0.5f, 0.5f, -0.5f);
    vertices[6] = new_vertex(0.5f, -0.5f, 0.5f);
    vertices[7] = new_vertex(-0.5f, -0.5f, 0.5f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void create_index_buffer() {
    unsigned int indices[] = {
        // Cube
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// -------------------------------------------



// -------------------------------------------
// Shader Setup

static void add_shader(GLuint shader_program, const char* p_shader_data, GLenum shader_type) {
    GLuint shader_obj = glCreateShader(shader_type);
    if(shader_obj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", shader_type);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = p_shader_data;

    GLint lengths[1]; // TODO: Maybe convert to GLuint instead to avoid narrowing conversion
    lengths[0] = strlen(p_shader_data);

    glShaderSource(shader_obj, 1, p, lengths);
    glCompileShader(shader_obj);

    GLint success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);

    if(!success) {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_obj, 1024, NULL, info_log);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader_type, info_log);
        exit(1);
    }

    glAttachShader(shader_program, shader_obj);
}

// Shader Files
// TODO: Use naming convention here
const char* pVertexShader = "./shaders/shader.vs";
const char* pFragmentShader = "./shaders/shader.fs";

static void compile_shaders() {
    // Create Program Handle
    GLuint shader_program = glCreateProgram();

    // Check to make sure handle isn't zero
    if(shader_program == 0) {
        fprintf(stderr, "Error creating the shader program!\n");
        exit(1);
    }

    // Read Vertex Shader Data from File
    char* vertex_shader = read_shader(pVertexShader);
    if(!vertex_shader)
        exit(1);
    add_shader(shader_program, vertex_shader, GL_VERTEX_SHADER);

    // Read Fragment Shader Data from File
    char* fragment_shader = read_shader(pFragmentShader);
    if(!fragment_shader)
        exit(1);
    add_shader(shader_program, fragment_shader, GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar error_log[1024] = {0};

    // Link shader program
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
        fprintf(stderr, "Error linking shader program: '%s'\n", error_log);
        exit(1);
    }

    // Validate shader information
    glValidateProgram(shader_program);
    glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, sizeof(error_log), NULL, error_log);
        fprintf(stderr, "Invalid shader program: '%s'\n", error_log);
        exit(1);
    }

    glUseProgram(shader_program);
}

// -------------------------------------------



// -------------------------------------------
// Callbacks

static void keyboard(unsigned char key, int mouse_x, int mouse_y) {
    cam_keyboard_input(&CAMERA_MAIN, key);
}

static void special_keyboard(int key, int mouse_x, int mouse_y) {
    cam_keyboard_input(&CAMERA_MAIN, key);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Rotate Mesh
    wt_rotate(&CUBE_WTRANSFORM, 0.0f, ROTATION_DELTA, 0.0f);

    // Initialize & Setup World-View-Perspective Matrices
    Matrix4f world = wt_get_matrix(&CUBE_WTRANSFORM);
    Matrix4f view = cam_get_matrix(&CAMERA_MAIN);
    Matrix4f perspective = persproj_get_matrix(&PERSPECTIVE);

    Matrix4f temp = mul(&perspective, &view);
    Matrix4f WVP = mul(&temp, &world);
    glUniformMatrix4fv(U_LOCALPOS, 1, GL_TRUE, &WVP.mat[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Position (Vertex->pos)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    // Color (Vertex->col)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay();
    glutSwapBuffers();
}

// -------------------------------------------

int main(int argc, char** argv) {
    // Randomize seed using PID
    srand(getpid());

    // Create GLUT Handle & Initialize Properties
    char* title = get_dynamic_title(VERSION);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(100,100);
    glutCreateWindow(title);
    free(title);

    // Initialize GL Extension Wrangler (GLEW)
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // Set clear color
    GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    glClearColor(r, g, b, a);

    // Enable & Configure Culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // Create Vertex Buffer, Index Buffer & Compile Shader Program
    create_vertex_buffer();
    create_index_buffer();
    compile_shaders();

    // Initialize Mesh Scale & Rotation
    wt_set_position(&CUBE_WTRANSFORM, 0.0f, 0.0f, 2.0f);
    wt_set_rotation(&CUBE_WTRANSFORM, 0.0f, 0.0f, 0.0f);
    wt_set_scale(&CUBE_WTRANSFORM, 1.0f);

    // Initialize Camera & Projection Matrices
    CAMERA_MAIN = new_camera();
    PERSPECTIVE = new_pers_projection(FOV, WINDOW_WIDTH, WINDOW_HEIGHT, ZNEAR, ZFAR);

    // Callbacks
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);

    glutMainLoop();
    return 0;
}