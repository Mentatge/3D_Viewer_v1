#ifndef S21_3DVIEWER_V1_H
#define S21_3DVIEWER_V1_H

#define _GNU_SOURCE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <GL/gl.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#endif
// #include <OpenGL/glext.h>

typedef struct Matrix {
  GLfloat **matrix;
  unsigned int rows;
  unsigned int cols;
} matrix_fm;

typedef struct facets {
  GLuint *vertices;
  GLuint number_of_vertices_in_facets;
} polygon_fm;

typedef struct data {
  GLuint count_of_vertices;
  GLuint count_of_facets;
  matrix_fm matrix_3d;
  polygon_fm *polygons;
} data_fm;

int parser(const char *filename, data_fm *value);
void memory_matrix(matrix_fm *name);
void free_memory_matrix(matrix_fm *name);
void memory_facet(data_fm *name);
void free_facet(data_fm *name);
void memory_polygon(const char *filename, data_fm *name);
int digit(char srt);
void free_polygon(data_fm *name);
// void rotation_by_ox(matrix_fm *A, double angle);
// void rotation_by_oy(matrix_fm *A, double angle);
// void rotation_by_oz(matrix_fm *A, double angle);
void normalisation(data_fm *value);
void rotation_by_ox(matrix_fm *A, GLfloat angle);
void rotation_by_oy(matrix_fm *A, GLfloat angle);
void rotation_by_oz(matrix_fm *A, GLfloat angle);

#endif
