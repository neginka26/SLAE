#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Num {
  long long numerator;
  long long denominator;
} num;

num **create_matrix(int m, int n);
void hilbert_matrix_5_order(num **hilbert_matrix);
void output_matrix_with_right_part(num **matrix, int m, int n);
void output_matrix_fractions(num **matrix, int m, int n);
void free_matrix(num **matrix, int m);
void lead_to_triangular(num **matrix, int dim, int dim_1);
num find_determ_of_matrix(num **matrix, int dim);
void direct_move_with_choise_main_el(num **matrix, int dim, int dim_1);
num* reverse_motion(num **matrix, int dim, int dim_1);

float get_float_num(num n);
num division_fractions(num n_1, num n_2);
num product_fractions(num n_1, num n_2);
num sum_fractions(num n_1, num n_2);
num difference_fractions(num n_1, num n_2);

long long find_greatest_common_divisor(num n);
num shorten_fraction(num n);

void output_array(num *array, int dim);

int main() {
  int dim = 5;
  num **matrix = create_matrix(dim, dim + 1);
  num **matrix_1 = create_matrix(dim, dim + 1);
  num determ = {1, 1};
  num *x = NULL;

  hilbert_matrix_5_order(matrix);
  hilbert_matrix_5_order(matrix_1);

  printf("Введена матрица:\n");
  output_matrix_with_right_part(matrix, dim, dim+1);

  lead_to_triangular(matrix, dim, dim+1);
  printf("\nТреугольная матрица:\n");
  output_matrix_with_right_part(matrix, dim, dim+1);

  determ = find_determ_of_matrix(matrix, dim);
  printf("\nОпределитель: %lld/%lld\n", determ.numerator, determ.denominator);

  if (get_float_num(determ) != 0) {
    printf("\nМетод Гаусса с выбором главного элемента:\n");
    direct_move_with_choise_main_el(matrix_1, dim, dim + 1);
    output_matrix_with_right_part(matrix_1, dim, dim + 1);
    output_matrix_fractions(matrix_1, dim, dim + 1);
    determ = find_determ_of_matrix(matrix_1, dim);
    printf("\nОпределитель: %lld/%lld\n", determ.numerator, determ.denominator);
    x = reverse_motion(matrix_1, dim, dim + 1);
    printf("Решение:\n");
    output_array(x, dim);
  } else {
    printf("Матрица невырожденная\n");
  }

  free_matrix(matrix_1, dim);
  free_matrix(matrix, dim);

  return 0;
}

float get_float_num(num n) {
    float number = (float) n.numerator / (float) n.denominator;
    return number;
}

// Выделение памяти для матрицы
num **create_matrix(int m, int n) {
  num **matrix = (num **)malloc(m * sizeof(num *));
  for (int i = 0; i < m; i++) {
    matrix[i] = (num *)malloc(n * sizeof(num));
  }
  return matrix;
}

// Заполнение матрицы как матрица Гильберта 5 порядка
void hilbert_matrix_5_order(num **hilbert_matrix) {
  int dim = 5;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim + 1; j++) {
      if (j == dim) {
        hilbert_matrix[i][j].numerator = i + 1;
        hilbert_matrix[i][j].denominator = 1;
      } else {
        hilbert_matrix[i][j].numerator = 1;
        hilbert_matrix[i][j].denominator = (i + 1) + (j + 1) - 1;
      }
    }
  }
}

// Вывод матрицы
void output_matrix_with_right_part(num **matrix, int m, int n) {
  float cur_num = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      cur_num = get_float_num(matrix[i][j]);
      if (j < n - 1) {
        printf("%f ", cur_num);
      } else if (j == n - 1 && i != m - 1) {
        printf(" | %f\n", cur_num);
      } else {
        printf("| %f", cur_num);
      }
    }
  }
  printf("\n");
}

// Очистка памяти, занимаемой матрицей
void free_matrix(num **matrix, int m) {
  for (int i = 0; i < m; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

// приведение к треугольному виду (метод Гаусса)
void lead_to_triangular(num **matrix, int dim, int dim_1) {
  num r = {1,1};
  for (int k = 0; k < dim; k++) {
    for (int i = k + 1; i < dim; i++) {
      r = division_fractions(matrix[i][k], matrix[k][k]); // matrix[i][k] / matrix[k][k]
      r = shorten_fraction(r);
      for (int j = k; j < dim_1; j++) {
        matrix[i][j] = difference_fractions(matrix[i][j], product_fractions(r, matrix[k][j]));  //matrix[i][j] - r * matrix[k][j];
        matrix[i][j] = shorten_fraction(matrix[i][j]);
      }
    }
  }
}

num division_fractions(num n_1, num n_2) {
  num res = {0, 1};
  res.numerator = n_1.numerator * n_2.denominator;
  res.denominator = n_1.denominator * n_2.numerator;
  return res;
}

num product_fractions(num n_1, num n_2) {
  num res = {0, 1};
  res.numerator = n_1.numerator * n_2.numerator;
  res.denominator = n_1.denominator * n_2.denominator;
  return res;
}

num sum_fractions(num n_1, num n_2) {
  num res = {n_1.numerator, n_1.denominator};
  if (n_1.denominator != n_2.denominator) {
    n_1.numerator *= n_2.denominator;
    n_1.denominator *= n_2.denominator;
    n_2.numerator *= res.denominator;
    n_2.denominator *= res.denominator;

    res.numerator = n_1.numerator + n_2.numerator;
    res.denominator = n_1.denominator;
  } else {
    res.numerator = n_1.numerator + n_2.numerator;
    res.denominator = n_1.denominator;
  }
  return res;
}

num difference_fractions(num n_1, num n_2) {
  num res = {n_1.numerator, n_1.denominator};
  if (n_1.denominator != n_2.denominator) {
    n_1.numerator *= n_2.denominator;
    n_1.denominator *= n_2.denominator;
    n_2.numerator *= res.denominator;
    n_2.denominator *= res.denominator;

    res.numerator = n_1.numerator - n_2.numerator;
    res.denominator = n_1.denominator;
  } else {
    res.numerator = n_1.numerator - n_2.numerator;
    res.denominator = n_1.denominator;
  }
  return res;
}

long long find_greatest_common_divisor(num n) {
    long long gcd = 1;
    int sign = 1;
    if (n.numerator < 0) sign = -1; 
    n.numerator *= sign;
    while (n.numerator != n.denominator) {
        if (n.numerator > n.denominator) n.numerator -= n.denominator;
        else if (n.numerator == 0) break;
        else n.denominator -= n.numerator;
    }
    n.numerator *= sign;
    gcd = n.denominator;
    return gcd;
}

num shorten_fraction(num n) {
    long long gcd = find_greatest_common_divisor(n); 
    n.numerator /= gcd;
    n.denominator /= gcd;
    return n;
}

num find_determ_of_matrix(num **matrix, int dim) {
  num determ = {1, 1};
  for (int i = 0; i < dim; i++) determ = product_fractions(determ, matrix[i][i]);
  return determ;
}

void direct_move_with_choise_main_el(num **matrix, int dim, int dim_1) {
  float max_el = 0;
  int i_max = 0;
  int j_max = 0;
  num *tmp_line = NULL;
  num tmp_el = {0, 1};
  num r = {0, 1};

  for (int k = 0; k < dim; k++) {
    // найти макисимум
    max_el = 0;
    for (int i = k; i < dim; i++) {
      for (int j = k; j < dim; j++) {
        if (fabs(get_float_num(matrix[i][j])) > max_el) {
          max_el = get_float_num(matrix[i][j]);
          i_max = i;
          j_max = j;
        }
      }
    }

    // swap строк
    tmp_line = matrix[k];
    matrix[k] = matrix[i_max];
    matrix[i_max] = tmp_line;

    // swap столбца
    for (int i = k; i < dim; i++) {
      tmp_el = matrix[i][k];
      matrix[i][k] = matrix[i][j_max];
      matrix[i][j_max] = tmp_el;
    }

    // зануление
    for (int i = k + 1; i < dim; i++) {
      r = division_fractions(matrix[i][k], matrix[k][k]); // matrix[i][k] / matrix[k][k]
      r = shorten_fraction(r);
      for (int j = k; j < dim_1; j++) {
        matrix[i][j] = difference_fractions(matrix[i][j], product_fractions(r, matrix[k][j]));  //matrix[i][j] - r * matrix[k][j];
        matrix[i][j] = shorten_fraction(matrix[i][j]);
      }
    }

    i_max = 0;
    j_max = 0;
  }
}

void output_matrix_fractions(num **matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j < n - 1) {
        printf("%lld/%lld ", matrix[i][j].numerator, matrix[i][j].denominator);
      } else if (j == n - 1 && i != m - 1) {
        printf(" | %lld/%lld\n", matrix[i][j].numerator, matrix[i][j].denominator);
      } else {
        printf("| %lld/%lld", matrix[i][j].numerator, matrix[i][j].denominator);
      }
    }
  }
  printf("\n");
}

num* reverse_motion(num **matrix, int dim, int dim_1) {
  num *answers = (num*)malloc(dim*sizeof(num));
  int ind = dim_1 - 1;
  num x = {0, 1};
  int diag_ind = dim - 1;
  for (int i = dim - 1; i >= 0; i--) {
    x = matrix[i][ind];
    for (int j = dim - 1; j > i; j--) {
      x = difference_fractions(x, product_fractions(matrix[i][j], answers[i+1]));
      x = shorten_fraction(x);
    }
    answers[i] = division_fractions(x, matrix[i][diag_ind]);
    answers[i] = shorten_fraction(answers[i]);
    diag_ind--;
  }
  return answers;
}

void output_array(num *array, int dim) {
  for (int i = 0; i < dim; i++) {
    if (i < dim - 1) printf("%lld/%lld ", array[i].numerator, array[i].denominator);
    else printf("%lld/%lld\n", array[i].numerator, array[i].denominator);
  }
}