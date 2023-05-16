#include <stdio.h>
#include <stdlib.h>

float **create_matrix(int m, int n);
void fill_matrix(float **matrix, int m, int n);
void output_matrix(float **matrix, int m, int n);
void free_matrix(float **matrix, int m);
void hilbert_matrix_5_order(float **hilbert_matrix);
void right_part(float **right_part);
float find_determ_of_matrix(float **matrix, int dim);
void remove_line_and_column(float **matrix, int dim, float **result_matrix,
                            int line, int column);
int check_for_triangular_matrix(float determ);
void lead_to_triangular(float **matrix, int dim);

int main() {
  int dimension = 3;
    // выделение памяти под матрицу
    float** matrix = create_matrix(dimension, dimension);
    // заполнение матрицы
    fill_matrix(matrix, dimension, dimension);
    // проверка: матрица вырожденная (определитель = 0)?
    if (check_for_triangular_matrix(find_determ_of_matrix(matrix, dimension))) {
        lead_to_triangular(matrix, dimension);
        output_matrix(matrix, dimension, dimension);
    } else {
        printf("Матрица невырожденная");
    }
  return 0;
}

// Выделение памяти для матрицы
float **create_matrix(int m, int n) {
  float **matrix = (float **)malloc(m * sizeof(float *));
  for (int i = 0; i < m; i++) {
    matrix[i] = (float *)malloc(n * sizeof(float));
  }
  return matrix;
}

// Заполнение матрицы с консоли
void fill_matrix(float **matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%f", &matrix[i][j]);
    }
  }
}

// Вывод матрицы
void output_matrix(float **matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j < n - 1) {
        printf("%f ", matrix[i][j]);
      } else if (j == n - 1 && i != m - 1) {
        printf("%f\n", matrix[i][j]);
      } else {
        printf("%f", matrix[i][j]);
      }
    }
  }
  printf("\n");
}

// Очистка памяти, занимаемой матрицей
void free_matrix(float **matrix, int m) {
  for (int i = 0; i < m; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

// Заполнение матрицы как матрица Гильберта 5 порядка
void hilbert_matrix_5_order(float **hilbert_matrix) {
  int dim = 5;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      hilbert_matrix[i][j] = 1.0 / ((i + 1) + (j + 1) - 1);
    }
  }
}

// Заполнение матрицы правой части
void right_part(float **right_part) {
  int column = 0;
  int line = 5;
  int counter = 1;
  for (int i = 0; i < line; i++) {
    right_part[i][column] = counter;
    counter++;
  }
}

float find_determ_of_matrix(float **matrix, int dim) {
  float determinant = 0;
  int i = 0;
  int sign = 1;
  int tmp_dim = dim - 1;
  if (dim < 1)
    printf("Ошибка: размерность матрицы < 1");
  else if (dim == 1) {
    determinant = matrix[0][0];
  } else if (dim == 2) {
    determinant = matrix[0][0] * matrix[1][1] - (matrix[0][1] * matrix[1][0]);
  } else if (dim > 2) {
    for (int j = 0; j < dim; j++) {
      float **tmp_matrix = create_matrix(tmp_dim, tmp_dim);
      remove_line_and_column(matrix, dim, tmp_matrix, i, j);
      determinant +=
          sign * matrix[i][j] * find_determ_of_matrix(tmp_matrix, tmp_dim);
      sign *= -1;
      free_matrix(tmp_matrix, tmp_dim);
    }
  }
  return determinant;
}

void remove_line_and_column(float **matrix, int dim, float **result_matrix,
                            int line, int column) {
  int line_counter = 0;
  int column_counter = 0;
  for (int i = 0; i < dim; i++) {
    column_counter = 0;
    if (i != line) {
      for (int j = 0; j < dim; j++) {
        if (j != column) {
          result_matrix[line_counter][column_counter] = matrix[i][j];
          column_counter++;
        }
      }
      line_counter++;
    }
  }
}

int check_for_triangular_matrix(float determ) {
  int is_triang = 0;
  if (determ == 0)
    is_triang = 0;
  else
    is_triang = 1;
  return is_triang;
}

void lead_to_triangular(float **matrix, int dim) {
  float r = 0;
  for (int k = 0; k < dim; k++) {
    for (int i = k + 1; i < dim; i++) {
      r = matrix[i][k] / matrix[k][k];
      for (int j = k; j < dim; j++) {
        matrix[i][j] = matrix[i][j] - r * matrix[k][j];
      }
    }
  }
}

// минусы: нет проверки деления на нуль в момент приведения к треугольному виду (при подборе коэффициента r не учитывается, что текущим элементом может быть нуль). Для случая с нулем нужно предусмотреть функцию смены строк местами.
