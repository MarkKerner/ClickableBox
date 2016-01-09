//
// Created by mark on 4.01.16.
//

#include "matrix.h"

/**
 * Builds a translation matrix in the same way as gluLookAt()
 * the resulting matrix is stored in pOut. pOut is returned.
 */
kmMat4 *lookAt(kmMat4 *pOut, const kmVec3 *pEye, const kmVec3 *pCenter, const kmVec3 *pUp) {
    kmVec3 f;
    kmVec3Subtract(&f, pCenter, pEye);
    kmVec3Normalize(&f, &f);

    kmVec3 s;
    kmVec3Cross(&s, &f, pUp);
    kmVec3Normalize(&s, &s);

    kmVec3 u;
    kmVec3Cross(&u, &s, &f);

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

    pOut->mat[12] = -kmVec3Dot(&s, pEye);
    pOut->mat[13] = -kmVec3Dot(&u, pEye);
    pOut->mat[14] = kmVec3Dot(&f, pEye);
    pOut->mat[15] = 1.0;

    return pOut;
}

void printMat4(const kmMat4 *mat) {
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
