#include "main.h"

static const int Width = 800;
static const int Height = 600;


int main() {

    GLFWwindow* window = initGlfwWindow();

    initGlew();

    GLuint mainVertexShader = createShader(GL_VERTEX_SHADER, mainVertexSource);
    GLuint mainFragmentShader = createShader(GL_FRAGMENT_SHADER, mainFragmentSource);

    GLuint mainShaderProgram = createShaderProgram(mainVertexShader, mainFragmentShader);

    GLuint boxVao;
    GLuint floorVao;
    glGenVertexArrays(1, &floorVao);
    glGenVertexArrays(1, &boxVao);
    createMainBufferObjects(boxVao, floorVao, mainShaderProgram);


    GLuint gridVertexShader = createShader(GL_VERTEX_SHADER, gridVertexSource);
    GLuint gridFragmentShader = createShader(GL_FRAGMENT_SHADER, gridFragmentSource);

    GLuint gridShaderProgram = createShaderProgram(gridVertexShader, gridFragmentShader);

    GLuint gridBoxVao;
    glGenVertexArrays(1, &gridBoxVao);

    createGridBufferObject(gridBoxVao, boxVertices, sizeof(boxVertices), gridShaderProgram);

    //Create texture
    loadBoxTexture(mainShaderProgram);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);

    TransformationMatrix transformationMatrix;
    setupTransformationMatrix(&transformationMatrix);

    GLint uniTransf = glGetUniformLocation(mainShaderProgram, "transf");
    GLint uniColor = glGetUniformLocation(mainShaderProgram, "overrideColor");
    GLint uniTransfGrid = glGetUniformLocation(gridShaderProgram, "transf");
    glUseProgram(0);


    float currentTime;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        //Clear the screen to white
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = (float) glfwGetTime();

        amat4_identity(&transformationMatrix.model);
        mat4RotationZ(&transformationMatrix.model, currentTime * 0.2f * degreesToRadians(180.0f));

        calculateTransformationMatrix(&transformationMatrix);

        glUseProgram(mainShaderProgram);

        glUniformMatrix4fv(uniTransf, 1, GL_FALSE, &transformationMatrix.transf.mat[0]);

        //Start with drawing functions

        glBindVertexArray(boxVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        if (GL_TRUE) {
            glUseProgram(gridShaderProgram);
            glUniformMatrix4fv(uniTransfGrid, 1, GL_FALSE, &transformationMatrix.transf.mat[0]);
            glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
            glBindVertexArray(gridBoxVao);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(0);
            glUseProgram(mainShaderProgram);
        }

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

        mat4 transl, scaling;
        amat4_translation(&transl, 0.0f, 0.0f, -1.0f);
        amat4_scaling(&scaling, 1.0f, 1.0f, -1.0f);
        mat4Multiply(&transformationMatrix.model, &transformationMatrix.model, &transl);
        mat4Multiply(&transformationMatrix.model, &transformationMatrix.model, &scaling);
        calculateTransformationMatrix(&transformationMatrix);
        glUniformMatrix4fv(uniTransfGrid, 1, GL_FALSE, &transformationMatrix.transf.mat[0]);
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

GLFWwindow* initGlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(Width, Height, "OpenGL tutorial", NULL, NULL);
    glfwMakeContextCurrent(window);
    initCallbacks(window);
    return window;
}

void initGlew() {
    glewExperimental = GL_TRUE;
    glewInit();
}

GLuint createShader(GLenum shaderType, const GLchar* shaderSrcString) {
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
    return shaderProgram;
}

void createMainBufferObject(GLuint vao, const GLfloat* vertices, GLint numVertices, GLuint shaderProgram) {

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    GLuint bufferObject;
    glGenBuffers(1, &bufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(
            posAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);


    assert(posAttrib != -1 && "posAttrib not found");


    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(
            colAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    assert(colAttrib != -1 && "posAttrib not found");

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glVertexAttribPointer(
            texAttrib, 2,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);


    assert(texAttrib != -1 && "posAttrib not found");

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void createGridBufferObject(GLuint vao, const GLfloat* vertices, GLint numVertices, GLuint shaderProgram) {

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    GLuint bufferObject;
    glGenBuffers(1, &bufferObject);
    printf("Bufferobject: %d", bufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(
            posAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);


    assert(posAttrib != -1 && "posAttrib not found");


    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(
            colAttrib, 3,
            GL_FLOAT, GL_FALSE,
            8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    assert(colAttrib != -1 && "colAttrib not found");

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void createMainBufferObjects(GLuint boxVao, GLuint floorVao, GLuint mainShaderProgram) {

    createMainBufferObject(floorVao, floorVertices, sizeof(floorVertices), mainShaderProgram);
    createMainBufferObject(boxVao, boxVertices, sizeof(boxVertices), mainShaderProgram);
}

void calculateTransformationMatrix(TransformationMatrix* transformationMatrix) {

    mat4Multiply(&transformationMatrix->modelView, &transformationMatrix->view, &transformationMatrix->model);

    mat4Multiply(&transformationMatrix->transf, &transformationMatrix->projection, &transformationMatrix->modelView);
}

void setupTransformationMatrix(TransformationMatrix* transformationMatrix) {

    //Generate an identity matrix
    mat4 model;
    amat4_identity(&model);

    //Generate view matrix
    mat4 view;
    GLfloat position = 3.0f;
    vec3 camera_position = {position, position, position};
    vec3 center_point = {0.0f, 0.0f, 0.0f};
    vec3 up_axis = {0.0f, 0.0f, 1.0f};
    lookAt(&view, &camera_position, &center_point, &up_axis);

    //Generate projection matrix
    mat4 proj;
    mat4PerspectiveProjection(&proj, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    transformationMatrix->model = model;
    transformationMatrix->view = view;
    transformationMatrix->projection = proj;

    calculateTransformationMatrix(transformationMatrix);
}

void initCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, &keyCallback);
    glfwSetCursorPosCallback(window, &cursorPosCallback);
    glfwSetMouseButtonCallback(window, &mouseButtonCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        printf("Left mouse clicked\n");
        double xf, yf;
        glfwGetCursorPos(window, &xf, &yf);
        int x, y;
        x = (GLint) xf;
        y = (GLint) yf;

        GLbyte color[4];
        GLfloat depth;
        GLuint index;

        glReadPixels(x, Height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels(x, Height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, Height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
               x, y, color[0], color[1], color[2], color[3], depth, index);
    }
}
