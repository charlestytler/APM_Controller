/*
 * linear_algebra.h - Library of linear algebra functions using float vector and matrices.
 * Created by Charlie Tytler  2018
 */

#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "math_types.h"

//
// Multiplies src vector by the provided scalar, storing the output in dest.
//
void scalar_multiply(const float scalar, const vector_t *src, vector_t *dest);

//
// Adds vector1 and vector2 and returns the result.
//
vector_t vector_add(const vector_t *vector1, const vector_t *vector2);

#endif
