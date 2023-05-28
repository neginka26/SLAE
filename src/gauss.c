#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **create_matrix(int m, int n);
void fill_matrix(float **matrix, int m, int n);
void output_matrix_with_right_part(float **matrix, int m, int n);
void free_matrix(float **matrix, int m);

float find_determ_of_matrix(float **matrix, int dim);
void remove_line_and_column(float **matrix, int dim, float **result_matrix,
                            int line, int column);
int check_for_triangular_matrix(float determ);
void lead_to_triangular(float **matrix, int dim, int dim_1);
float* reverse_motion(float **matrix, int dim, int dim_1);

void output_array(float *array, int dim);

void hilbert_matrix_5_order(float **hilbert_matrix);

void direct_move_with_choise_main_el(float **matrix, int dim, int dim_1);

typedef struct Num {
  int numerator;
  int denominator;
} num;

int main() {
  // int dim = 3;
  int dim = 5;
  float *x = NULL;
  float **matrix = create_matrix(dim, dim + 1);
  hilbert_matrix_5_order(matrix);
  // fill_matrix(matrix, dim, dim+1);
  printf("\nВведена СЛАУ:\n");
  output_matrix_with_right_part(matrix, dim, dim + 1);
  float determ = find_determ_of_matrix(matrix, dim);
  printf("\nОпределитель:\n%0.20f\n", determ);

  if (check_for_triangular_matrix(find_determ_of_matrix(matrix, dim))) {
    // lead_to_triangular(matrix, dim, dim + 1);
    // printf("\nТреугольный вид матрицы:\n");
    // output_matrix_with_right_part(matrix, dim, dim + 1);
    // float *x = reverse_motion(matrix, dim, dim+1);
    // printf("\nРешение системы:\n");
    // output_array(x, dim);
    
    printf("\nВыборка по главному элементу матрицы:\n");
    direct_move_with_choise_main_el(matrix, dim, dim+1);
    output_matrix_with_right_part(matrix, dim, dim+1);
    float *x = reverse_motion(matrix, dim, dim+1);
    printf("\nРешение системы:\n");
    output_array(x, dim);

  } else {
    printf("\nМатрица невырожденная\n");
  }
  free_matrix(matrix, dim);
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
void output_matrix_with_right_part(float **matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j < n - 1) {
        printf("%f ", matrix[i][j]);
      } else if (j == n - 1 && i != m - 1) {
        printf(" | %f\n", matrix[i][j]);
      } else {
        printf("| %f", matrix[i][j]);
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

// нахождение определителя матрицы рекурсивным методом
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

// удаление одной линии и одной колонки
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

//проверка на возможность приведения матрицы к треугольному виду
int check_for_triangular_matrix(float determ) {
  int is_triang = 0;
  if (determ == 0)
    is_triang = 0;
  else
    is_triang = 1;
  return is_triang;
}

// приведение к треугольному виду (метод Гаусса)
void lead_to_triangular(float **matrix, int dim, int dim_1) {
  float r = 0;
  for (int k = 0; k < dim; k++) {
    for (int i = k + 1; i < dim; i++) {
      r = matrix[i][k] / matrix[k][k];
      for (int j = k; j < dim_1; j++) {
        matrix[i][j] = matrix[i][j] - r * matrix[k][j];
      }
    }
  }
}

// обратный ход и нахождение X-ов
float* reverse_motion(float **matrix, int dim, int dim_1) {
    float *answers = (float*)malloc(dim * sizeof(float));
    int ind = dim_1 - 1;
    float x = 0;
    int diag_ind = dim - 1;
    for (int i = dim-1; i >= 0; i--) {
        x = matrix[i][ind];
        for (int j = dim-1; j > i; j--) {
            x -= matrix[i][j] * answers[i+1]; 
        }
        answers[i] = x / matrix[i][diag_ind];
        diag_ind--;
    }
    return answers;
}

// вывод массива (одномерного)
void output_array(float *array, int dim) {
    for (int i = 0; i < dim; i++) {
        if (i < dim - 1) printf("%f ", array[i]);
        else printf("%f\n", array[i]);
    }
}

// Заполнение матрицы как матрица Гильберта 5 порядка
void hilbert_matrix_5_order(float **hilbert_matrix) {
  int dim = 5;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim+1; j++) {
      if (j == dim)  hilbert_matrix[i][j] = i+1;
      else hilbert_matrix[i][j] = 1.0 / ((i + 1) + (j + 1) - 1);
    }
  }
}

// прямая проходка с выбором главного элемента по всей матрицы
void direct_move_with_choise_main_el(float **matrix, int dim, int dim_1) {
  int i_max = 0;
  int j_max = 0;
  float max_el = 0;
  float r = 0;
  float *tmp_line = NULL;
  float tmp_el = 0;


  for (int k = 0; k < dim; k++) {
    // нахождение максимума (по модулю) матрицы
    max_el = 0;
    for (int i = k; i < dim; i++) {
      for (int j = k; j < dim; j++) {
        if (fabs(matrix[i][j]) > max_el) {
          max_el = fabs(matrix[i][j]);
          i_max = i;
          j_max = j;
        }
      }
    }

    // swap строки
    tmp_line = matrix[k];
    matrix[k] = matrix[i_max];
    matrix[i_max] = tmp_line;

    // swap столбца
    for (int i = k; i < dim; i++) {
      tmp_el = matrix[i][k];
      matrix[i][k] = matrix[i][j_max];
      matrix[i][j_max] = tmp_el;
    }

    // исключение столбцов (зануление)
    for (int i = k+1; i < dim; i++) {
      r = matrix[i][k]/matrix[k][k];
      for (int j = k; j < dim_1; j++) {
        matrix[i][j] -= matrix[k][j] * r;
      }
    }

    i_max = 0;
    j_max = 0;
  }
}
