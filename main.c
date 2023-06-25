#define GLEW_STATIC

#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

#include "src/include/util.h"
#include "src/include/math_3d.h"

GLuint VBO;
GLint U_TRANSFORM;

static void create_vertex_buffer() {
    Vector3f Vertices[3];
    Vertices[0] = new_vec3f(-1.0f, -1.0f, 0.0f);    // Bottom left
    Vertices[1] = new_vec3f(1.0f, -1.0f, 0.0f);     // Bottom right
    Vertices[2] = new_vec3f(0.0f, 1.0f, 0.0f);      // Top

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
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
    U_TRANSFORM = glGetUniformLocation(shader_program, "transform");

    if(U_TRANSFORM == -1) {
        printf("Error getting uniform 'transform' in Vertex Shader.\n");
        exit(1);
    }

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

    // -----------------------------------
    // Uniform Tests
    // TODO: Add Matrix Multiplication Operator Overload


    // -----------------------------------
    // Scale

//    static float scale = 1.0f;
//    static float delta = 0.0005f;
//
//    scale += delta;
//    if(scale >= 1.5f || scale <= 0.5f)
//        delta *= -1.0f;
//
//    Matrix4f scale_transform = new_matrix4f(scale, 0.0f, 0.0f, 0.0f,    // X
//                                        0.0f, scale, 0.0f, 0.0f,        // Y
//                                        0.0f, 0.0f, scale, 0.0f,        // Z
//                                        0.0f, 0.0f, 0.0f, 1.0f);        // W
//    glUniformMatrix4fv(U_TRANSFORM, 1, GL_TRUE, &scale_transform.mat[0][0]);

    // -----------------------------------


    // -----------------------------------
    // Translation

//    static float scale = 0.0f;
//    static float delta = 0.0005f;
//    scale += delta;
//
//    if(scale >= 1.0f || scale <= -1.0f)
//        delta *= -1.0f;
//
//    Matrix4f translation = new_matrix4f(1.0f, 0.0f, 0.0f, scale * 2,    // X
//                                        0.0f, 1.0f, 0.0f, scale,        // Y
//                                        0.0f, 0.0f, 1.0f, 0.0f,         // Z
//                                        0.0f, 0.0f, 0.0f, 1.0f);        // W
//    glUniformMatrix4fv(U_TRANSFORM, 1, GL_TRUE, &translation.mat[0][0]); // GL_TRUE = Row-Major Order | GL_FALSE = Column-Major Order

    // -----------------------------------


    // -----------------------------------
    // Rotation

//    static float theta_radians = 0.0f;
//    static float delta = 0.01f;
//
//    theta_radians += delta;
//    if(theta_radians >= 1.5708f || theta_radians <= -1.5708f)
//        delta *= -1.0f;
//
//    Matrix4f rotation = new_matrix4f(cosf(theta_radians), -sinf(theta_radians), 0.0f, 0.0f,    // X
//                                        sinf(theta_radians), cosf(theta_radians), 0.0f, 0.0f,  // Y
//                                        0.0f, 0.0f, 1.0f, 0.0f,     // Z
//                                        0.0f, 0.0f, 0.0f, 1.0f);    // W
//    glUniformMatrix4fv(U_TRANSFORM, 1, GL_TRUE, &rotation.mat[0][0]);

    // -----------------------------------

    // END Uniform Tests
    // -----------------------------------

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glutPostRedisplay();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // Create GLUT Handle & Initialize Properties
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024,1024);
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

    // Create Vertex Buffer & Compile Shader Program
    create_vertex_buffer();
    compile_shaders();
    glutDisplayFunc(draw);

    glutMainLoop();
    return 0;
}