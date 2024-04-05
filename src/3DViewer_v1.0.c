#include "3DViewer_v1.0.h"

/*!
 * \mainpage
 * \author miltmire mentatge concetts
 * \version 1.0
 * \section intro_sec 3DViewer_v1.0 was done with
 * 1. C language, C++ language \n
 * 2. Qt Creator, Qt
 * 3. Qcustomplot, OpenGL
 * 4. Qmake6
 * 5. Doxygen
 */

/**
 * @brief Парсим .obj файл в собсвенную структуру
 * @param filename Имя/путь файла
 * @param value Стуктура, которая заполняется вершинами и полигонами
 * @return Функция возвращает код ошибки
 */

int parser(const char *filename, data_fm *value) {
  FILE *file = NULL;
  size_t len = 256;
  char *buffer = NULL;
  char new_buf[256];
  int i = 1;
  int l = 0;
  int p = 0;
  int m = 1;
  int return_value = 1;
  int space = 0;

  if ((file = fopen(filename, "r"))) {
    return_value = 1;
  } else {
    return_value = 0;
  }

  value->count_of_facets = 0;
  value->count_of_vertices = 0;

  if ((file != NULL) && (return_value)) {
    while (getline(&buffer, &len, file) != -1) {
      if (buffer[0] == 'v' && !(buffer[1] == 'n' || buffer[1] == 't')) {
        value->count_of_vertices += 1;
      } else if (buffer[0] == 'f') {
        value->count_of_facets += 1;
      }
      // free(buffer);
    }
  } else {
    return_value = 0;
  }

  value->matrix_3d.cols = 3;
  value->matrix_3d.rows = value->count_of_vertices + 1;

  memory_matrix(&value->matrix_3d);
  memory_facet(value);
  memory_polygon(filename, value);

  // free(buffer);

  if (file != NULL) fclose(file);

  if ((file = fopen(filename, "r"))) {
    return_value = 1;
  } else {
    return_value = 0;
  }

  if (file != NULL) {
    while (getline(&buffer, &len, file) != -1) {
      if (buffer[0] == 'v' && !(buffer[1] == 'n' || buffer[1] == 't')) {
        sscanf(buffer, "v %f %f %f", &value->matrix_3d.matrix[i][0],
               &value->matrix_3d.matrix[i][1], &value->matrix_3d.matrix[i][2]);
        i++;
      } else if (buffer[0] == 'f') {
        p = 0;

        for (int k = 0; buffer[k] != '\0'; k++) {
          if (digit(buffer[k]) && buffer[k - 1] == ' ') {
            while (digit(buffer[k])) {
              new_buf[l] = buffer[k];
              l++;
              k++;
              space = 1;
            }
            l = 0;
            // printf("%d\n", value->polygons[m]->vertices[p]);
          }
          if ((buffer[k] == ' ' && space == 1) ||
              (buffer[k] == '\n' && space == 1)) {
            sscanf(new_buf, "%d", &value->polygons[m].vertices[p]);
            memset(new_buf, '\0', 256);
            space = 0;
            p++;
          }
        }
        m++;
      }
      // free(buffer);
    }
    // printf("%lf\n", value->matrix_3d.matrix[1][2]);
    normalisation(value);
    // printf("%lf\n", value->matrix_3d.matrix[1][2]);
  }

  free(buffer);
  if (file != NULL) fclose(file);
  // printf("%d\n", value->count_of_vertices);
  // printf("%d\n", value->count_of_facets);
  // printf("%lf\n", value->matrix_3d->matrix[35290][0]);
  // printf("%d\n", value->polygons[35288]->vertices[3]);
  // // printf("%lf\n", value->matrix_3d->matrix[106829][1]);
  // // printf("%lf\n", value->matrix_3d->matrix[106829][2]);
  return return_value;
}

/**
 * @brief Выделяем память для матрицы вершин
 * @param name Имя стуктуры
 */

void memory_matrix(matrix_fm *name) {
  name->matrix = (float **)calloc((name->rows + 1), sizeof(float *));
  for (unsigned int i = 0; i < name->rows; i++) {
    name->matrix[i] = (float *)calloc(name->cols, sizeof(float));
  }
}

/**
 * @brief Освобождаем память матрицы вершин
 * @param name Имя стуктуры
 */

void free_memory_matrix(matrix_fm *name) {
  for (unsigned int i = 0; i < (name->rows + 1); i++) {
    void *adress = name->matrix[i];
    free(adress);
  }
  void *adress = name->matrix;
  free(adress);
}

/**
 * @brief Выделяем память для всех полигонов вершин
 * @param name Имя стуктуры
 */

void memory_facet(data_fm *name) {
  name->polygons =
      (polygon_fm *)calloc((name->count_of_facets + 1), sizeof(polygon_fm));
}

/**
 * @brief Освобождаем память полигонов вершин
 * @param name Имя стуктуры
 */

void free_facet(data_fm *name) { free(name->polygons); }

/**
 * @brief Выделяем память для каждого полигона вершин
 * @param name Имя стуктуры
 */

void memory_polygon(const char *filename, data_fm *name) {
  size_t len = 256;
  char *buffer = NULL;
  FILE *file = fopen(filename, "r");
  GLuint count = 0;
  int j = 1;

  if (file != NULL) {
    while (getline(&buffer, &len, file) != -1) {
      if (buffer[0] == 'f') {
        for (int i = 0; buffer[i] != '\0'; i++) {
          if (digit(buffer[i])) {
            if (buffer[i - 1] == ' ') {
              count++;
            }
          }
        }
        name->polygons[j].vertices = (unsigned int *)calloc(count, sizeof(int));
        name->polygons[j].number_of_vertices_in_facets = count;
        j++;
        count = 0;
      }
      // free(buffer);
    }
  }

  if (file != NULL) fclose(file);
}

/**
 * @brief Освобождаем память для кажого полигона вершин
 * @param name Имя стуктуры
 */

void free_polygon(data_fm *name) {
  for (unsigned int i = 1; i <= name->count_of_facets; i++) {
    free(name->polygons->vertices);
  }
}

/**
 * @brief Проверка на число
 * @param str Строка, получаемая из файла
 */

int digit(char str) { return (str >= '0' && str <= '9'); }

// void rotation_by_ox(matrix_fm *A, double angle) {
//     for (unsigned int i = 0; i < A->rows; i++) {
// 		double temp_y = A->matrix[i][1];
// 		double temp_z = A->matrix[i][2];
// 		A->matrix[i][1] = cos(angle) * temp_y - sin(angle) * temp_z;
// 		A->matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
// 	}
// }

// void rotation_by_oy(matrix_fm *A, double angle) {
//     for (unsigned int i = 0; i < A->rows; i++) {
// 		double temp_x = A->matrix[i][0];
// 		double temp_z = A->matrix[i][2];
// 		A->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_z;
// 		A->matrix[i][2] = sin(angle) * temp_x + cos(angle) * temp_z;
// 	}
// }

/**
 * @brief Нормализует координаты вершин
 * @param value Структура с вершинами и полигонами
 */

void normalisation(data_fm *value) {
  double max = 0;
  for (unsigned int i = 0; i < value->matrix_3d.rows; i++) {
    for (unsigned int j = 0; j < value->matrix_3d.cols; j++) {
      if (fabs(value->matrix_3d.matrix[i][j]) > max) {
        max = fabs(value->matrix_3d.matrix[i][j]);
      }
    }
  }
  for (unsigned int i = 0; i < value->matrix_3d.rows; i++) {
    for (unsigned int j = 0; j < value->matrix_3d.cols; j++) {
      value->matrix_3d.matrix[i][j] /= max;
    }
  }
}

// void rotation_by_ox(matrix_fm *A, GLfloat angle) {
//     angle = angle * M_PI / 180;
//     for (unsigned int i = 0; i < A->rows; i++) {
//     double temp_y = A->matrix[i][1];
//     double temp_z = A->matrix[i][2];
//         A->matrix[i][1] = cos(angle) * temp_y - sin(angle) * temp_z;
//     A->matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
//   }
// }

// void rotation_by_oy(matrix_fm *A, GLfloat angle) {
//     angle = angle * M_PI / 180;
//     for (unsigned int i = 0; i < A->rows; i++) {
//     double temp_x = A->matrix[i][0];
//     double temp_z = A->matrix[i][2];
//     A->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_z;
//     A->matrix[i][2] = sin(angle) * temp_x + cos(angle) * temp_z;
//   }
// }

// void rotation_by_oz(matrix_fm *A, GLfloat angle) {
//     angle = angle * M_PI / 180;
//     for (unsigned int i = 0; i < A->rows; i++) {
//     double temp_x = A->matrix[i][0];
//     double temp_y = A->matrix[i][2];
//     A->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_y;
//     A->matrix[i][1] = sin(angle) * temp_x + cos(angle) * temp_y;
//   }
// }

/**
 * @brief Поворот по оси OX
 * @param A Структура с вершинами и полигонами
 * @param angle Угол поворота
 */

void rotation_by_ox(matrix_fm *A, GLfloat angle) {
  angle = angle * M_PI / 180;
  for (unsigned int i = 0; i < A->rows; i++) {
    double temp_y = A->matrix[i][1];
    double temp_z = A->matrix[i][2];
    A->matrix[i][1] = cos(angle) * temp_y + sin(angle) * temp_z;
    A->matrix[i][2] = -1 * sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

/**
 * @brief Поворот по оси OY
 * @param A Структура с вершинами и полигонами
 * @param angle Угол поворота
 */

void rotation_by_oy(matrix_fm *A, GLfloat angle) {
  angle = angle * M_PI / 180;
  for (unsigned int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_z = A->matrix[i][2];
    A->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_z;
    A->matrix[i][2] = -1 * sin(angle) * temp_x + cos(angle) * temp_z;
  }
}

/**
 * @brief Поворот по оси OZ
 * @param A Структура с вершинами и полигонами
 * @param angle Угол поворота
 */

void rotation_by_oz(matrix_fm *A, GLfloat angle) {
  angle = angle * M_PI / 180;
  for (unsigned int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_y = A->matrix[i][1];
    A->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_y;
    A->matrix[i][1] = -1 * sin(angle) * temp_x + cos(angle) * temp_y;
  }
}
