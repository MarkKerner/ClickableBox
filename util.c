//
// Created by mark on 3.01.16.
//
#include "util.h"


int checkShader(GLuint shader) {
    GLint status;
    char buffer[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(shader, 1024, NULL, buffer);

    if (status == GL_TRUE) {
        printf("%s\n", "Shader compilation successful!");
        printf("%s\n", buffer);
        return 0;
    } else {
        printf("%s\n", "Shader compilation failed!");
        printf("%s\n", buffer);
        return 1;
    }
}

void loadBoxTexture(GLuint shaderProgram) {
    //Create texture
    GLuint texture;
    glGenTextures(1, &texture);

    int width, height;
    unsigned char *image;

    //Load image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image("sample.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texBox"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
