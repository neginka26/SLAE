#include <stdio.h>
#include <stdlib.h>

float **create_matrix(int m, int n);
void fill_matrix(float **matrix, int m, int n);
void output_matrix(float **matrix, int m, int n);
void free_matrix(float **matrix, int m);
void hilbert_matrix_5_order(float **hilbert_matrix);
void right_part(float **right_part);
void gaussian_elimination_method(float **left_matrix, float **right_matrix,
                                 int columns, int lines);

int main() {
  int m = 5; // количество строк
  int c = 1; // количество столбцов
  // выделяем память под матрицы
  float **hilb_matrix = create_matrix(m, m);
  float **right_part_matrix = create_matrix(m, c);
  // заполняем матрицы гильберта и правую часть
  hilbert_matrix_5_order(hilb_matrix);
  right_part(right_part_matrix);
  //вывод матриц
  printf("Матрица Гильберта 5 порядка:\n");
  output_matrix(hilb_matrix, m, m);
  printf("\nПравая часть:\n");
  output_matrix(right_part_matrix, m, c);
  // освобождение памяти
  free_matrix(hilb_matrix, m);
  free_matrix(right_part_matrix, m);
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

void gaussian_elimination_method(float **left_matrix, float **right_matrix,
                                 int columns, int lines) {}
