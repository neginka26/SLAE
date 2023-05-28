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
void free_matrix(num **matrix, int m);
void lead_to_triangular(num **matrix, int dim, int dim_1);
num find_determ_of_matrix(num **matrix, int dim);
float get_float_num(num n);

num division_fractions(num n_1, num n_2);
num product_fractions(num n_1, num n_2);
num sum_fractions(num n_1, num n_2);
num difference_fractions(num n_1, num n_2);

long long find_greatest_common_divisor(num n);
num shorten_fraction(num n);

int main() {
  int dim = 5;
  num **matrix = create_matrix(dim, dim + 1);
  num **matrix_1 = create_matrix(dim, dim + 1);
  num determ = {1, 1};

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
    output_matrix_with_right_part(matrix_1, dim, dim + 1);
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
  res = shorten_fraction(res);
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
  res = shorten_fraction(res);
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
    while (n.numerator != n.denominator) {
        if (n.numerator > n.denominator) n.numerator -= n.denominator;
        else if (n.numerator == 0) break;
        else n.denominator -= n.numerator;
    }
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