#define GLEW_STATIC
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#include <stdio.h>
#include <math.h>
#include <process.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "src/include/util.h"
#include "src/include/math_3d.h"

GLuint VBO;
GLuint IBO;
GLint U_TRANSFORM;
GLint U_WORLDPOS;

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

    // -----------------------------------
    // Uniform Testing

    // Universal Transform Modifier
//    U_TRANSFORM = glGetUniformLocation(shader_program, "transform");
//
//    if(U_TRANSFORM == -1) {
//        printf("Error getting uniform 'scale' in Vertex Shader.\n");
//        exit(1);
//    }

    // -----------------------------------

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

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // -----------------------------------------
    // Perspective Projection Testing

    static float scale = 0.0f;
    scale += 0.0001f;

    Matrix4f rotation = new_matrix4f(cosf(scale), 0.0f, -sinf(scale), 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          sinf(scale), 0.0f, cosf(scale), 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f translation = new_matrix4f(1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 2.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f);

    float FOV = 90.0f;
    float tanHalfFOV = tanf(radians(FOV / 2.0f));
    float d = 1/tanHalfFOV;
    float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    // View Frustum Calculations
    float near_plane = 1.0f;
    float far_plane = 10.0f;
    float dist = near_plane - far_plane;

    float A = (-far_plane - near_plane) / dist;
    float B = 2.0f * far_plane * near_plane / dist;

    Matrix4f projection = new_matrix4f(d / aspect_ratio, 0.0f, 0.0f, 0.0f,
                                        0.0f, d, 0.0f, 0.0f,
                                        0.0f, 0.0f, A, B,
                                        0.0f, 0.0f, 1.0f, 0.0f);

    Matrix4f temp = mul(&projection, &translation);
    Matrix4f final = mul(&temp, &rotation);
    glUniformMatrix4fv(U_WORLDPOS, 1, GL_TRUE, &final.mat[0][0]);

    // -----------------------------------------

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

int main(int argc, char** argv) {
    // Randomize seed using PID
    srand(getpid());

    // Create GLUT Handle & Initialize Properties
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Vortex Engine v0.0.1");

    // Initialize GL Extension Wrangler (GLEW)
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // Set clear color
    GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    glClearColor(r, g, b, a);

    // Enable Culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // Create Vertex Buffer, Index Buffer & Compile Shader Program
    create_vertex_buffer();
    create_index_buffer();
    compile_shaders();

    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}