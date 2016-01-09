//
// Created by mark on 3.01.16.
//
#ifndef MAIN_H
#define MAIN_H

#include <SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <kazmath/kazmath.h>

#include "util.h"


//Calbacks
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


//Initializing
GLFWwindow* initGlfwWindow();

void initGlew();

void initCallbacks(GLFWwindow* window);

//Shaders
const GLchar* mainVertexSource =
        "#version 150 core\n"

                "in vec3 position;"
                "in vec3 color;"
                "in vec2 texcoord;"

                "out vec3 Color;"
                "out vec2 Texcoord;"

                "uniform mat4 transf;"
                "uniform vec3 overrideColor;"

                "void main() {"
                "	Texcoord = texcoord;"
                "	Color = overrideColor * color;"
                "	gl_Position = transf * vec4(position, 1.0);"
                "}";

const GLchar* mainFragmentSource =
        "#version 150 core\n"
                "in vec3 Color;"
                "in vec2 Texcoord;"
                "out vec4 outColor;"
                "uniform sampler2D texBox;"
                "void main() {"
                "		vec4 texColor = texture(texBox, Texcoord);"
                "		outColor = vec4(Color, 1.0) * texColor;"
                "}";

const GLchar* gridVertexSource =
        "#version 150 core\n"

                "in vec3 position;"
                "in vec3 color;"

                "out vec3 Color;"

                "uniform mat4 transf;"
                "uniform vec3 overrideColor;"

                "void main() {"
                "	Color = overrideColor * color;"
                "	gl_Position = transf * vec4(position, 1.0);"
                "}";

const GLchar* gridFramentSource =
        "#version 150 core\n"
                "in vec3 Color;"
                "out vec4 outColor;"
                "void main() {"
                "		outColor = vec4(Color, 1.0);"
                "}";

const GLfloat boxVertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
};

const GLfloat floorVertices[] = {
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};


GLuint createShader(GLenum shaderType, const GLchar* shaderSrcString);

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

void createMainBufferObjects(GLuint boxVao, GLuint floorVao, GLuint mainShaderProgram);
void createBufferObject(GLuint vao, const GLfloat* vertices, GLuint shaderProgram);

typedef struct TransformationMatrix {
    kmMat4 projection;
    kmMat4 view;
    //projection * view matrix multiplication
    kmMat4 projView;
    kmMat4 model;
    //projView * model matrix multiplication
    kmMat4 transf;
} TransformationMatrix;

void setupTransformationMatrix(GLuint shaderProgram, TransformationMatrix* tm);

void mainloop(GLFWwindow* window, TransformationMatrix tm, GLuint shaderProgram);

#endif //MAIN_H
