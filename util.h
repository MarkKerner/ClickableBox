//
// Created by mark on 3.01.16.
//

#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <SOIL.h>
#include <GL/glew.h>


//Check if shader compiled successfully
int checkShader(GLuint shader);
void loadBoxTexture(GLuint shaderProgram);

#endif //UTIL_H