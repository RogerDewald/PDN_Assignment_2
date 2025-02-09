#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

/* ----------- Project 2 - Problem 1 - Matrix Mult -----------

    This file will multiply two matricies.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //

void parseCSV(FILE *input, long int *output);

int main(int argc, char *argv[]) {
  // Catch console errors
  if (argc != 10) {
    printf("USE LIKE THIS: "
           "parallel_mult_mat_mat file_A.csv n_row_A n_col_A file_B.csv n_row_"
           "B n_col_B result_matrix.csv time.csv num_threads \n");
    return EXIT_FAILURE;
  }

  // Get the input files
  FILE *inputMatrix1 = fopen(argv[1], "r");
  FILE *inputMatrix2 = fopen(argv[4], "r");

  char *p1;
  char *p2;

  // Get matrix 1's dims
  int n_row1 = strtol(argv[2], &p1, 10);
  int n_col1 = strtol(argv[3], &p2, 10);

  // Get matrix 2's dims
  int n_row2 = strtol(argv[5], &p1, 10);
  int n_col2 = strtol(argv[6], &p2, 10);

  // Get num threads
  int thread_count = strtol(argv[9], NULL, 10);

  // Get output files
  FILE *outputFile = fopen(argv[7], "w");
  FILE *outputTime = fopen(argv[8], "w");

  // TODO: malloc the two input matrices and the output matrix
  // Please use long int as the variable type
  long int *mat_1 = (long int *)malloc((n_row1 * n_col1) * sizeof(int));
  long int *mat_2 = (long int *)malloc((n_row2 * n_col2) * sizeof(int));
  long int *out_mat = (long int *)malloc((n_row1 * n_col2) * sizeof(int));

  // TODO: Parse the input csv files and fill in the input matrices
  parseCSV(inputMatrix1, mat_1);
  parseCSV(inputMatrix2, mat_2);

  // We are interesting in timing the matrix-matrix multiplication only
  // Record the start time
  double start = omp_get_wtime();

  // TODO: Parallelize the matrix-matrix multiplication
#pragma omp parallel for num_threads(thread_count)
  for (int i = 0; i < n_row1; i++) {
    for (int j = 0; j < n_col2; j++) {
      long int sum = 0;
      for (int a = 0; a < n_col1; a++) {
        sum += mat_1[i * n_col1 + a] * mat_2[a * n_col2 + j];
      }
      out_mat[i * n_col2 + j] = sum;
    }
  }

  // Record the finish time
  double end = omp_get_wtime();

  // Time calculation (in seconds)
  double time_passed = end - start;

  // Save time to file
  fprintf(outputTime, "%f", time_passed);

  // TODO: save the output matrix to the output csv file
  int count = 0;
  for (int i = 0; i < n_row1; i++) {
    for (int j = 0; j < n_col2; j++) {
      if (j == n_col2 - 1) {
        fprintf(outputFile, "%ld", out_mat[count]);
      } else {
        fprintf(outputFile, "%ld,", out_mat[count]);
      }
      count++;
    }
    fprintf(outputFile, "\n");
  }

  // Cleanup
  fclose(inputMatrix1);
  fclose(inputMatrix2);
  fclose(outputFile);
  fclose(outputTime);
  // Remember to free your buffers!
  free(mat_1);
  free(mat_2);
  free(out_mat);

  return 0;
}

void parseCSV(FILE *input, long int *output) {
  char vector_row_line[4000];
  int vec_index = 0;
  while (fgets(vector_row_line, sizeof(vector_row_line), input)) {
    char *nl = strchr(vector_row_line, '\n');
    if (nl) {
      *nl = '\0';
    }
    char *token = strtok(vector_row_line, ",");
    while (token != NULL) {
      output[vec_index] = atoi(token);
      vec_index++;
      token = strtok(NULL, ",");
    }
  }
}
