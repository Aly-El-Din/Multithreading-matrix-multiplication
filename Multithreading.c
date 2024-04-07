#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<unistd.h>
#include<sys/time.h>

int** matrix1;
int** matrix2;
int result[20][20];
int rows1;
int cols1;
int rows2;
int cols2;

struct dimension{
    int r;
    int c;
};

void* elementMultiplication(void* ptr){
    struct dimension* dims=(struct dimension*)ptr;
    int row=dims->r;
    int col=dims->c;
   
    int res=0;
    for(int i=0;i<cols1;i++){
        res+=matrix1[row][i]*matrix2[i][col];
    }
    result[row][col]=res;
}


void multiplyPerElement(){
    // Check if the matrices can be multiplied
    if (cols1 != rows2) {
        printf("Matrices cannot be multiplied\n");
        return;
    }

    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            result[i][j]=0;
        }
    }
    //Creating thread for each row

    pthread_t threads[400][400];

    struct timeval stop,start;
    gettimeofday(&start,NULL);
    
    for(int i=0;i<rows1;i++){
        for(int j=0;j<cols2;j++){
            struct dimension* dimensions=malloc(sizeof(struct dimension));
            dimensions->r=i;
            dimensions->c=j;
            pthread_create(&threads[i][j],NULL,elementMultiplication,(void*)dimensions);
        }
    }
    
    
    
    for (int i = 0; i < rows1; i++) {
        for(int j=0;j<cols2;j++){
            pthread_join(threads[i][j], NULL);
        }
    }
    
    gettimeofday(&stop,NULL);
    printf("Number of threads created for multiplying per element: %d\n",rows1*cols2);

    printf("Seconds taken for multiplying per element: %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken for multiplying per element:: %lu\n", stop.tv_usec - start.tv_usec);

    printf("\n======================\n");

    FILE* file = fopen("c_per_element.txt", "w");
    if (file == NULL) {
        printf("Could not open file");
        return ;
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            fprintf(file, "%d ", result[i][j]); // Write result to file
        }
        fprintf(file, "\n"); // Move to the next line in the file
    }
    fclose(file);
    return;
}




void* rowMultiplication(void* arg) {
    int row_num = *((int*)arg);
    
    for (int j = 0; j < cols2; j++) {
        for (int k = 0; k < cols1; k++) {
            result[row_num][j] += matrix1[row_num][k] * matrix2[k][j];
        }
    }
    
    return NULL;
}

void multiplyPerRow() {
    // Check if the matrices can be multiplied
    if (cols1 != rows2) {
        printf("Matrices cannot be multiplied\n");
        return;
    }

    //Creating thread for each row

    pthread_t* threads = malloc(rows1 * sizeof(pthread_t));
    if (threads == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    int* row_nums = malloc(rows1 * sizeof(int));
    if (row_nums == NULL) {
        printf("Memory allocation failed\n");
        free(threads);
        return;
    }


    struct timeval stop,start;
    gettimeofday(&start,NULL);

    for (int i = 0; i < rows1; i++) {
        row_nums[i] = i;
        pthread_create(&threads[i], NULL, rowMultiplication, &row_nums[i]);
    }
     for (int i = 0; i < rows1; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&stop,NULL);
    
    printf("Number of threads created for multiplying per row: %d\n",rows1);

    printf("Seconds taken for multiplying per row: %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken for multiplying per row: %lu\n", stop.tv_usec - start.tv_usec);

    printf("\n===================\n\n");

    FILE* file = fopen("c_per_row.txt", "w");
    if (file == NULL) {
        printf("Could not open file");
        return ;
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            fprintf(file, "%d ", result[i][j]); // Write result to file
        }
        fprintf(file, "\n"); // Move to the next line in the file
    }
    fclose(file);



    return;
}

void multiplyPerMatrix() {
    // Check if the matrices can be multiplied
    if (cols1 != rows2) {
        printf("Matrices cannot be multiplied\n");
        return;
    }

    // Open the file for writing
    FILE* file = fopen("c_per_matrix.txt", "w");
    if (file == NULL) {
        printf("Could not open file for writing\n");
        return;
    }

    //Counting variables for execution time

    struct timeval stop,start;
    gettimeofday(&start,NULL);

    // Perform matrix multiplication and write the result to the file
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int res = 0;
            for (int k = 0; k < cols1; k++) {
                res += matrix1[i][k] * matrix2[k][j];
            }
            fprintf(file, "%d ", res); // Write result to file
        }
        fprintf(file, "\n"); // Move to the next line in the file
    }
    gettimeofday(&stop,NULL);
    
    printf("Number of threads created for multiplying per matrix: 1\n");
    printf("Seconds taken for multiplying per matrix:%lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken for multiplying per matrix:%lu\n", stop.tv_usec - start.tv_usec);
    printf("\n========================\n\n");
    // Close the file
    fclose(file);

    

}

int** readMatrixFromFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    // Read dimensions
    fscanf(file, "row=%d col=%d ", rows, cols);  // Note the space after %d

    // Allocate memory for the matrix
    int** matrix = (int**)malloc(*rows * sizeof(int*));
    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < *rows; i++) {
        matrix[i] = (int*)malloc(*cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    // Read matrix elements
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
    return matrix;
}

int main() {
    const char* filename1 = "a.txt";
    const char* filename2 = "b.txt";

    // Read matrix from file
    matrix1 = readMatrixFromFile(filename1, &rows1, &cols1);
    matrix2 = readMatrixFromFile(filename2, &rows2, &cols2);

    
    multiplyPerMatrix();
    multiplyPerRow();
    multiplyPerElement();
   
 
    

    return 0;
}
