#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Takes in the dimensions of the matrix and allocates space to it
 *Returns a double** */
double ** createMatrix(int n, int k){
	double **matrix;
	int i;
	matrix = (double**)malloc(sizeof(double*) * n);
	for(i = 0; i < n; i++){
		matrix[i] = (double*)malloc(sizeof(double) * k);
	}
	return matrix;
}

//Frees Matrix
void freeMatrix(double** matrix, int n)
{
    int i;
    double* current;
    for(i = 0; i < n; i++)
    {
        current = matrix[i];
        free(current);
        current = NULL;
    }
    free(matrix);
    matrix = NULL;
}

/*Prints a matrix -> used for debugging purpose mostly but was modified at the end to print the final answer*/
void printMatrix(double **matrix, int n, int k){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < k; j++){
			printf("%0.0lf", matrix[i][j]);
		}
		printf("\n");
	}
	return;
}

//transpose a matrix
double** transpose(double** matrix, int n, int m){
    int k,l;
    double ** transMatrix = createMatrix(m, n); //Creates a matrix

    //Transpose the matrix
    for(k = 0; k < m; k++){
        for(l = 0; l < n; l++){
            transMatrix[k][l] = matrix[l][k];
        }
    }
    return transMatrix; //Returns the transposed matrix
}

//Mulitple 2 matrix
double** multiply(double** matrix1, int n, int m, double** matrix2, int n2, int m2){
    int i,j,k;
    double** mult = createMatrix(n, m2); //creates a matrix

    //Multiplies the matrix
     for(k = 0; k < n; k++){
         for(i = 0; i < m2; i++){
             mult[k][i] = 0.0;
             for(j = 0; j < m; j++){
                 mult[k][i] += matrix1[k][j] * matrix2[j][i];
             }
         }
     }
     return mult;
}

double** inverse(double** matrix, int n)
{
    int i, j, k, v, count = n;
    double counter; //A counter to hold the number to divide the each index of a row by this number
    double** finalInverse = createMatrix(n, n); //Allocates space
    double** inverMatrix = createMatrix(n ,n*2); //Alocates space
    
    //Create the matrix that is combined with the arugment matrix and the identity matrix
    for(i = 0; i < n; i++){
        for(j = 0; j < (2*n); j++){
            if(j == count){
                inverMatrix[i][j] = 1.000000;
            }
            else if(j > (n-1)){
                inverMatrix[i][j] = 0.000000;
            }
            else{
                inverMatrix[i][j] = matrix[i][j];
            }
        }
        count++;
    }
    /*Gauss-Jordan Elimination
     * Checks diagonals if = 1
     * computes lower half = 0
     * computes higher half = 0 */
    //Foward pass
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){   
            if(i > j){
                continue;
            }
            if(i==j && inverMatrix[i][j] != 1.000000){ //Checks pivot != 1
                   counter = inverMatrix[i][j]; 
                   for(k = 0; k < (2*n); k++){
                       inverMatrix[i][k] = inverMatrix[i][k] / counter;
                   }
                   continue;
            }
            else if(i!=j && inverMatrix[j][i] != 0.000000){
                for(v = j; v < n; v++){   
                    counter = inverMatrix[v][i];
                    for(k = 0; k < (2*n); k++){
                        inverMatrix[v][k] = inverMatrix[v][k] + ((-1*counter)* inverMatrix[j-1][k]);
                    }
                }
                continue;
            }
        }
    }
    //Backwards pass
    for(i = (n-2); i > -1; i--){
        for(j = (n-1); inverMatrix[i][j] != 1.000000; j--){
            for(v = i; v > -1; v--){
                counter = inverMatrix[i][j];
                for(k = 0; k < (2*n); k++){
                    inverMatrix[v][k] = inverMatrix[v][k] + ((-1*counter)* inverMatrix[j][k]);
                }
            }
        }
    }
    //Inputs the augmented matrix into a new matrix
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            finalInverse[i][j] = inverMatrix[i][j+n];
        }
    }
    freeMatrix(inverMatrix, n); //free inverMatrix
    return finalInverse; //return finalInverse;
}

//Predicts the cost of the house
double** predict(double** matrix1, int n1, int m1, double** matrix2, int n2, int m2)
{
    int i,j;
    double** predicted = createMatrix(n1, 1); //Alocates space

    for(i = 0; i < n1; i++){
        for(j = 0; j < n2 ;j++){
            if(j == 0){
                predicted[i][0] = matrix2[j][0];
                continue;
            }
            else{
                predicted[i][0] += (matrix2[j][0] * matrix1[i][j-1]);
                continue;
            }
        }
    }
    return predicted;
}

int main(int argc, char *argv[]){
	//Does error check for the amount of arguments
	if (argc != 3){
		printf("error\n");
		exit(0);
	}

	//Opens the training and data file
	FILE *fp = fopen(argv[1], "r");
	FILE *ptr = fopen(argv[2], "r");
	if(fp == NULL || ptr == NULL){	//File does not exist
		printf("error\n");
		exit(0);
	}
	char word[10];
	int k, n, k1, n1, i, j;
	double number;
	
	//reads the first line which is a word -> has no purpose for the assignment
	fscanf(fp, "%s\n", word);
	fscanf(ptr, "%s\n", word);

	//reads the next line for int k
	fscanf(fp, "%d\n", &k);
	fscanf(ptr, "%d\n", &k1);

	//reads the next line for int n
	fscanf(fp, "%d\n", &n);
	fscanf(ptr, "%d\n", &n1);

	//Allocates space for X, Y and Data matrix
	double **X = createMatrix(n, k+1);
	double **Y = createMatrix(n, 1);
	double **Data = createMatrix(n1, k1);

	//Reads and inputs the numbers into the X, Y and Data matrices
	for(i = 0; i < n; i++){
		for(j = 0; j < k+2; j++){
			if(j == 0){	//First column is all 1.0's
				X[i][j] = 1.0;
				if(i < n1){
					fscanf(ptr, "%lf ", &number);
					Data[i][j] = number;
				}
			}
			else if(j == k+1){	//Adds the inputs to the Y matrix
				fscanf(fp, "%lf\n", &number);
				Y[i][0] = number;
			}else{	//Continues the X matrix
				fscanf(fp, "%lf ", &number);
				X[i][j] = number;
				if(i < n1 && j == k1-1){ //Data: if j is at the last index for data
					fscanf(ptr, "%lf\n", &number);
					Data[i][j] = number;				}
				else if(i < n1 && j < k1){ //Data: continues to fill in the Data matrix
					fscanf(ptr, "%lf ", &number);
                    Data[i][j] = number;
				}
			}
		}
	}
	fclose(fp); fclose(ptr); //Closes both files

	double **transposeMatrix = transpose(X, n, k+1); //Tranposed matrix
	double **multMatrix = multiply(transposeMatrix, (k+1), n, X, n, (k+1));	//final product of x^t * x
	double **inverseMatrix = inverse(multMatrix, k+1);	//inverseMatrix
	double **finishE = multiply(inverseMatrix, k+1, k+1, transposeMatrix, k+1, n);	//finishs the equations
	double **weights = multiply(finishE, k+1, n, Y, n, 1);	//solves for w and = it to weight
	double **housePrice = predict(Data, n1, k1, weights, k+1, 1);	//Predicts the house price
	printMatrix(housePrice, n1, 1);	//Prints the matrix out
	
	//Frees all allocated memory
    freeMatrix(X, n);
    freeMatrix(Y, n);
    freeMatrix(Data, n1);
    freeMatrix(transposeMatrix, k+1);
    freeMatrix(multMatrix, k+1);
    freeMatrix(inverseMatrix, k+1);
    freeMatrix(finishE, k+1);
    freeMatrix(weights, k+1);
    freeMatrix(housePrice, n1);
	return 0;
}