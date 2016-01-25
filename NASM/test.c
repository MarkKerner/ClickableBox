#include <stdio.h>

typedef struct {
    float x, y, z;
} vec3;

typedef struct mat4 {
	float mat[16];
} mat4;


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

void printVec3(const vec3* vec) {
	printf("Vector: %f, %f, %f\n", vec->x, vec->y, vec->z);
}


extern void vec3_subtract(vec3* pOut, vec3* pM1, vec3* pM2);
extern void vec3_assign(vec3* pM1, vec3* pM2);

extern void mat4_identity(float* mat);
extern void mat4_scale(float* mat, float x, float y, float z);
extern void mat4_translation(float* mat, float x, float y, float z);


int main () {
	vec3 vector;
	vec3 v1 = {4.0f, 52.0f, 8.0f};
	vec3 v2 = {1.0f, 2.0f, 3.0f};
	vec3_subtract(&vector, &v1, &v2);
	printVec3(&vector);

	vec3_assign(&vector, &v1);
	printVec3(&vector);

	vec3_assign(&vector, &v2);
	printVec3(&vector);

	mat4 matrix;

	mat4_identity(matrix.mat);
	printMat4(&matrix);

	mat4 matrix2;
	printf("mat4 addr = 0x%0x \n", matrix2.mat);
	mat4_translation(matrix.mat, 1, 0, 0);

	mat4_scale(matrix.mat, 12.0f, 7.0f, 8.0f);
	printMat4(&matrix);

	mat4_translation(matrix.mat, 5.0f, 3.0f, 1.0f);
	printMat4(&matrix);

	return 0;
}