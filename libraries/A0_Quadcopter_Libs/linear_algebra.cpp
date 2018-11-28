/*
 * linear_algebra.cpp - Library of linear algebra functions using float vector and matrices.
 * Created by Charlie Tytler  2018
 */

#include "linear_algebra.h"

void scalar_multiply(const float scalar, const vector_t *src, vector_t *dest)
{
    dest->x = src->x * scalar;
    dest->y = src->y * scalar;
    dest->z = src->z * scalar;
}

vector_t vector_add(const vector_t *vector1, const vector_t *vector2)
{
    vector_t result;
    result.x = vector1->x + vector2->x;
    result.y = vector1->y + vector2->y;
    result.z = vector1->z + vector2->z;
    return result;
}
