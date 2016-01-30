//
// Created by mark on 4.01.16.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct mat4 {
    float mat[16];
} mat4;

float square(float n);

float vec3Length(const vec3* pIn);

vec3* vec3Normalize(vec3* pOut, vec3* pIn);

vec3* vec3Cross(vec3* pOut, const vec3* pV1, const vec3* pV2);

float vec3Dot(const vec3* pV1, const vec3* pV2);

mat4* mat4RotationZ(mat4* pOut, const float radians);

mat4* mat4Multiply(mat4* pOut, const mat4* pM1, const mat4* pM2);

mat4* lookAt(mat4* pOut, const vec3* pEye, const vec3* pCenter, const vec3* pUp);

mat4* mat4PerspectiveProjection(mat4* pOut, float fovY,
                                float aspect, float zNear,
                                float zFar);

float degreesToRadians(float degrees);

//Print matrix
void printMat4(const mat4* mat);

//Assembly functions
extern void avec3_subtract(vec3* pOut, const vec3* pM1, const vec3* pM2);

extern void avec3_assign(vec3* pM1, vec3* pM2);

extern void amat4_identity(mat4* mat);

extern void amat4_scaling(mat4* mat, float x, float y, float z);

extern void amat4_translation(mat4* mat, float x, float y, float z);


#endif //MATRIX_H
