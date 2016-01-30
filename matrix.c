//
// Created by mark on 4.01.16.
//

#include "matrix.h"

/**
 * Builds a translation matrix in the same way as gluLookAt()
 * the resulting matrix is stored in pOut. pOut is returned.
 */

float square(float n) {
    return n * n;
}

float vec3Length(const vec3* pIn) {
    return sqrtf(square(pIn->x) + square(pIn->y) + square(pIn->z));
}


vec3* vec3Normalize(vec3* pOut, vec3* pIn) {
    if (!pIn->x && !pIn->y && !pIn->z)
        avec3_assign(pOut, pIn);

    float l = 1.0f / vec3Length(pIn);

    vec3 v;
    v.x = pIn->x * l;
    v.y = pIn->y * l;
    v.z = pIn->z * l;

    pOut->x = v.x;
    pOut->y = v.y;
    pOut->z = v.z;

    return pOut;
}

vec3* vec3Cross(vec3* pOut, const vec3* pV1, const vec3* pV2) {

    vec3 v;

    v.x = (pV1->y * pV2->z) - (pV1->z * pV2->y);
    v.y = (pV1->z * pV2->x) - (pV1->x * pV2->z);
    v.z = (pV1->x * pV2->y) - (pV1->y * pV2->x);

    pOut->x = v.x;
    pOut->y = v.y;
    pOut->z = v.z;

    return pOut;
}

float vec3Dot(const vec3* pV1, const vec3* pV2) {
    return (pV1->x * pV2->x
            + pV1->y * pV2->y
            + pV1->z * pV2->z);
}


mat4* mat4RotationZ(mat4* pOut, const float radians) {
    pOut->mat[0] = cosf(radians);
    pOut->mat[1] = sinf(radians);
    pOut->mat[2] = 0.0f;
    pOut->mat[3] = 0.0f;

    pOut->mat[4] = -sinf(radians);
    pOut->mat[5] = cosf(radians);
    pOut->mat[6] = 0.0f;
    pOut->mat[7] = 0.0f;

    pOut->mat[8] = 0.0f;
    pOut->mat[9] = 0.0f;
    pOut->mat[10] = 1.0f;
    pOut->mat[11] = 0.0f;

    pOut->mat[12] = 0.0f;
    pOut->mat[13] = 0.0f;
    pOut->mat[14] = 0.0f;
    pOut->mat[15] = 1.0f;

    return pOut;
}

mat4* mat4Multiply(mat4* pOut, const mat4* pM1, const mat4* pM2) {
    float mat[16];

    const float* m1 = pM1->mat, * m2 = pM2->mat;

    mat[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    mat[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    mat[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    mat[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

    mat[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    mat[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    mat[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    mat[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

    mat[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    mat[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    mat[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    mat[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

    mat[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    mat[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    mat[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    mat[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];


    memcpy(pOut->mat, mat, sizeof(float) * 16);

    return pOut;
}

mat4* lookAt(mat4* pOut, const vec3* pEye, const vec3* pCenter,
             const vec3* pUp) {
    vec3 f;
    avec3_subtract(&f, pCenter, pEye);
    vec3Normalize(&f, &f);

    vec3 s;
    vec3Cross(&s, &f, pUp);
    vec3Normalize(&s, &s);

    vec3 u;
    vec3Cross(&u, &s, &f);

    pOut->mat[0] = s.x;
    pOut->mat[1] = u.x;
    pOut->mat[2] = -f.x;
    pOut->mat[3] = 0.0;

    pOut->mat[4] = s.y;
    pOut->mat[5] = u.y;
    pOut->mat[6] = -f.y;
    pOut->mat[7] = 0.0;

    pOut->mat[8] = s.z;
    pOut->mat[9] = u.z;
    pOut->mat[10] = -f.z;
    pOut->mat[11] = 0.0;

    pOut->mat[12] = -vec3Dot(&s, pEye);
    pOut->mat[13] = -vec3Dot(&u, pEye);
    pOut->mat[14] = vec3Dot(&f, pEye);
    pOut->mat[15] = 1.0;

    return pOut;
}


float degreesToRadians(float degrees) {
    return degrees * (float) (M_PI / 180.0f);
}

mat4* mat4PerspectiveProjection(mat4* pOut, float fovY,
                                float aspect, float zNear,
                                float zFar) {
    float r = degreesToRadians(fovY / 2);
    float deltaZ = zFar - zNear;
    float s = sinf(r);
    float cotangent = 0;

    if (deltaZ == 0 || s == 0 || aspect == 0) {
        return NULL;
    }

    /*cos(r) / sin(r) = cot(r)*/
    cotangent = cosf(r) / s;

    amat4_identity(pOut);
    pOut->mat[0] = cotangent / aspect;
    pOut->mat[5] = cotangent;
    pOut->mat[10] = -(zFar + zNear) / deltaZ;
    pOut->mat[11] = -1;
    pOut->mat[14] = -2 * zNear * zFar / deltaZ;
    pOut->mat[15] = 0;

    return pOut;
}

void printMat4(const mat4* mat) {
    const int max = 4;
    printf("\n\n");
    for (int i = 0; i < max; i++) {
        printf("|");

        for (int j = 0; j < max; j++) {
            if (j > 0) {
                printf("\t");
            }

            printf("%f", mat->mat[i + max * j]);
        }

        printf("|\n");
    }
    printf("\n");
}
