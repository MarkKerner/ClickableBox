//
// Created by mark on 4.01.16.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <kazmath/kazmath.h>

kmMat4 *lookAt(kmMat4 *pOut, const kmVec3 *pEye, const kmVec3 *pCenter, const kmVec3 *pUp);

//Print matrix
void printMat4(const kmMat4 *mat);

#endif //MATRIX_H
