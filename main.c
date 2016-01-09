#include <GL/glew.h>
#include "main.h"
#include "matrix.h"

static const int Width = 800;
static const int Height = 600;


int main() {
    GLFWwindow *window = initGlfwWindow();
    initGlew();


    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    GLuint boxVao;
    GLuint floorVao;

    glGenVertexArrays(1, &floorVao);
    glGenVertexArrays(1, &boxVao);
    createBufferObjects(boxVao, floorVao);
    //Setup uniform values
    setupDefaultAttribPointers(shaderProgram, boxVao);
    setupDefaultAttribPointers(shaderProgram, floorVao);

    //Create texture
    loadBoxTexture(shaderProgram);

    //Enable depth and stencil test
    glEnable(GL_DEPTH_TEST);

    TransformationMatrix transformationMatrix;
    setupTransformationMatrix(shaderProgram, &transformationMatrix);


    GLint uniTransf = glGetUniformLocation(shaderProgram, "transf");
    GLint uniColor = glGetUniformLocation(shaderProgram, "overrideColor");


    glEnable(GL_DEPTH_TEST);
    printMat4(&transformationMatrix.transf);
    float currentTime;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        //Clear the screen to white
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = (float) glfwGetTime();

        //Make the texture smaller and bigger
        kmMat4Identity(&transformationMatrix.model);

        kmMat4RotationZ(&transformationMatrix.model, currentTime * 0.2f * kmDegreesToRadians(180.0f));
        kmMat4Multiply(&transformationMatrix.transf, &transformationMatrix.projView, &transformationMatrix.model);
        glUniformMatrix4fv(uniTransf, 1, GL_FALSE, &transformationMatrix.transf.mat[0]);

        glBindVertexArray(boxVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //
        glEnable(GL_STENCIL_TEST);

        // Draw floor
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
        glDepthMask(GL_FALSE);
        glClear(GL_STENCIL_BUFFER_BIT);

        glBindVertexArray(floorVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //Draw refelction
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDepthMask(GL_TRUE);

        kmMat4 transl, scaling;
        kmMat4Translation(&transl, 0.0f, 0.0f, -1.0f);
        kmMat4Scaling(&scaling, 1.0f, 1.0f, -1.0f);
        kmMat4Multiply(&transformationMatrix.model, &transformationMatrix.model, &transl);
        kmMat4Multiply(&transformationMatrix.model, &transformationMatrix.model, &scaling);
        kmMat4Multiply(&transformationMatrix.transf, &transformationMatrix.projView, &transformationMatrix.model);
        glUniformMatrix4fv(uniTransf, 1, GL_FALSE, &transformationMatrix.transf.mat[0]);
        glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);

        glBindVertexArray(boxVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
        //
        glDisable(GL_STENCIL_TEST);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *initGlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(Width, Height, "OpenGL tutorial", NULL, NULL);
    glfwMakeContextCurrent(window);
    initCallbacks(window);
    return window;
}

void initGlew() {
    glewExperimental = GL_TRUE;
    glewInit();
}

GLuint createShader(GLenum shaderType, const GLchar *shaderSrcString) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrcString, NULL);
    glCompileShader(shader);
    checkShader(shader);

    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
//    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    return shaderProgram;
}

void createBufferObjects(GLuint boxVao, GLuint floorVao) {

    glBindVertexArray(floorVao);
    GLuint floorBufferObject;
    glGenBuffers(1, &floorBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, floorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glBindVertexArray(boxVao);
    GLuint boxBufferObject;
    glGenBuffers(1, &boxBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, boxBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);


    glBindVertexArray(0);
}

void setupDefaultAttribPointers(GLuint shaderProgram, GLuint vao) {
    glBindVertexArray(vao);
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(
            posAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(
            colAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void *) (3 * sizeof(float)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(
            texAttrib, 2,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void *) (6 * sizeof(float)));

    glBindVertexArray(0);
}

void setupTransformationMatrix(GLuint shaderProgram, TransformationMatrix *transformationMatrix) {
    // Final matrix
    kmMat4 transf;
    kmMat4 projView;
    //Generate an identity matrix
    kmMat4 model;
    kmMat4Identity(&model);

    //Generate view matrix
    kmMat4 view;
    GLfloat position = 2.5f;
    kmVec3 camera_position = {position, position, position};
    kmVec3 center_point = {0.0f, 0.0f, 0.0f};
    kmVec3 up_axis = {0.0f, 0.0f, 1.0f};
    lookAt(&view, &camera_position, &center_point, &up_axis);

    //Generate projection matrix
    kmMat4 proj;
    kmMat4PerspectiveProjection(&proj, 45.0f, 800.0f / 600.0f, 1.0f, 10.0f);


    kmMat4Multiply(&projView, &proj, &view);
    kmMat4Multiply(&transf, &projView, &model);

    GLint uniTransf = glGetUniformLocation(shaderProgram, "transf");
    glUniformMatrix4fv(uniTransf, 1, GL_FALSE, &transf.mat[0]);

    transformationMatrix->projection = proj;
    transformationMatrix->view = view;
    transformationMatrix->projView = projView;
    transformationMatrix->model = model;
    transformationMatrix->transf = transf;
}

void initCallbacks(GLFWwindow *window) {
    glfwSetKeyCallback(window, &keyCallback);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
