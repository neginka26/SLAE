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
  int m = 5; // количество строк
  int c = 1; // количество столбцов
  // int n = 3;
  //  выделяем память под матрицы
  float **hilb_matrix = create_matrix(m, m);
  float **right_part_matrix = create_matrix(m, c);
  // float **matrix = create_matrix(n, n);
  //  заполняем матрицы гильберта и правую часть
  hilbert_matrix_5_order(hilb_matrix);
  right_part(right_part_matrix);
  // fill_matrix(matrix, n, n);
  //вывод матриц
  printf("Матрица Гильберта 5 порядка:\n");
  output_matrix(hilb_matrix, m, m);
  printf("\nПравая часть:\n");
  output_matrix(right_part_matrix, m, c);
  // printf("\nМатрица 3 на 3:\n");
  // output_matrix(matrix, n, n);
  //  детерминант (определитель)
  float determ = 0;
  determ = find_determ_of_matrix(hilb_matrix, m);
  printf("\nОпределитель: \n%.20f\n", determ);
  if (check_for_triangular_matrix(determ)) {
    printf("\nМатрица может быть приведена к треугольному виду\n");
    lead_to_triangular(hilb_matrix, m);
    output_matrix(hilb_matrix, m, m);
  } else {
    printf("Матрица не может быть треугольной\n");
  }
  // освобождение памяти
  free_matrix(hilb_matrix, m);
  free_matrix(right_part_matrix, m);
  // free_matrix(matrix, n);
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
    for (int j = k + 1; j < dim; j++) {
      r = matrix[j][k] / matrix[k][k];
      for (int i = k; i < dim; i++) {
        matrix[j][i] = matrix[j][i] - r * matrix[k][i];
      }
    }
  }
}
