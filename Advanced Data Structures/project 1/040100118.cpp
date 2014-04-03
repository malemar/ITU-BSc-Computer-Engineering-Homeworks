/****************************************/
/*	BLG381E - Advanced Data Structures	*/
/*				Fall 2012				*/
/*										*/
/*			Volkan Ilbeyli				*/
/*			   040100118				*/
/*										*/
/*		Insturctor: Zehra Cataltepe		*/
/****************************************/
/*										*/
/*				Not: 90					*/

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

void mergeSort(int**, int, int);
void merge(int**, int, int, int);
void insertionSort(int**, int);

int main(){
	int** hw_Matrix;	//Nx2 Matrix
	int hw_count;		//N
	char c;				//algorithm choice

	//random seed
	srand((unsigned)time(NULL));

	//prompt user to enter N
	cout << "Enter the number of Homeworks: " << endl;
	cin >> hw_count;
	
	//memory allocation for the matrix
	hw_Matrix = new int*[hw_count];
	for(int i = 0 ; i < hw_count ; i++)
		hw_Matrix[i] = new int[2];

	//data constructing for the matrix
	for(int i = 0 ; i < hw_count ; i++){
		hw_Matrix[i][0] = i+1;					//matrice index number = i+1
		hw_Matrix[i][1] = rand()%hw_count + 1;	// 1 < Homework time < homework count
	}

	//print the matrix
	cout << endl;
	for(int i = 0 ; i < hw_count ; i++)
		cout << hw_Matrix[i][0] << "\t" << hw_Matrix[i][1] << endl;
	cout << endl <<"Matrix is created: " << hw_count << "x2" << endl;

	//get the algorithm to be performed
	int start_time, end_time;
	const int DIVIDER = 30000;
	int limit = DIVIDER/hw_count;	//algorithm will run time equation:
									// 30000 / N = loop limit, N = input count	
	bool terminate = false;			//larger inputs will run less times
	while(terminate != true){		//algorithm will run "limit" times
		cout << "Choose the sort algorithm (I / M): " << endl;
		cin >> c;
		switch(c){
		case 'i':
		case 'I':
			insertionSort(hw_Matrix, hw_count);		//time copmutation is inside the function
			terminate = true;
			break;

		case 'M':
		case 'm':
			start_time = clock();
			for(int k = 0 ; k < limit ; k++)
				mergeSort(hw_Matrix, 0, hw_count-1);
			end_time = clock();

			//print the running time
			cout << (double)(end_time - start_time)/limit << " ms" << endl; 
			fflush(stdin);
			getchar();
			
			terminate = true;
			break;

		default:
			cout << "Error: Wrong input!" << endl << endl;
			break;
		}
	}
	
	//print the sorted matrix
	terminate = false;
	while(terminate == false){
		cout << endl << "Print the sorted matrix? (y/n): ";
		cin >> c;
		switch(c){
		case 'y':
		case 'Y':
			for(int i = 0 ; i < hw_count ; i++)
				cout << hw_Matrix[i][0] << "\t" << hw_Matrix[i][1] << endl;
			terminate = true;
			break;
		
		case 'n':
		case 'N':
			terminate = true;
			break;
		
		default:
			cout << "Wrong input";
			break;
		}
	}
	
	//memory deallocation
	for(int i = 0 ; i < hw_count ; i++)
		delete[] hw_Matrix[i];
	delete[] hw_Matrix;

	return 0;
}

void mergeSort(int** matrix, int lbound, int ubound){
	
	int midbound;
	if(lbound < ubound){	
		midbound = (lbound + ubound)/2;
		mergeSort(matrix, lbound, midbound);
		mergeSort(matrix, midbound+1, ubound);
		merge(matrix, lbound, midbound, ubound);
	}

	return;
}

void merge(int** matrix, int lbound, int midbound, int ubound){
	int i,j,k;
	
	int length1 = midbound - lbound + 1;		//1
	int length2 = ubound - midbound;			//1
	unsigned const long int infinity = 999999999;

	//create sub-matrices
	int** L = new int*[length1 + 1];
	int** R = new int*[length2 + 1];
	for(int i = 0 ; i < length1 + 1 ; i++)
		L[i] = new int[2];
	for(int i = 0 ; i < length2 + 1 ; i++)
		R[i] = new int[2];

	//assign values to sub-matrices
	for(i = 0 ; i < length1 ; i++){
		L[i][1] = matrix[lbound + i][1];
		L[i][0] = matrix[lbound + i][0];
	}
	L[length1][1] = infinity;

	for(i = 0 ; i < length2 ; i++){
		R[i][1] = matrix[midbound + 1 + i][1];
		R[i][0] = matrix[midbound + 1 + i][0];
	}
	R[length2][1] = infinity;

	/*
	//KONTROL
	cout << endl << "submatrice1: " << endl;
	for(int a = 0 ; a < length1 ; a++){
		cout << R[a][1] << endl;
	}

	cout << "submatrice2: " << endl;
	for(int a = 0 ; a < length2 ; a++){
		cout << L[a][1] << endl;
	}
	*/

	//merge sub-matrices
	i = 0;		j = 0;
	for(k = lbound ; k < ubound+1 ; k++){
		if(L[i][1] <= R[j][1]){
			matrix[k][1] = L[i][1];
			matrix[k][0] = L[i][0];
			i++;
		}
		else{
			matrix[k][1] = R[j][1];
			matrix[k][0] = R[j][0];
			j++;
		}
	}

}

void insertionSort(int** matrix, int size){

	//create the original matrix which will be used
	//to reset the sorted matrix
	int** original_matrix = new int*[size];
	for(int i = 0 ; i < size ; i++)
		original_matrix[i] = new int[2];
	
	//assign the values
	for(int i = 0 ; i < size ; i++){
		original_matrix[i][0] = matrix[i][0];
		original_matrix[i][1] = matrix[i][1];
	}


	int start_time = clock();
	const int DIVIDER = 50000;
	int limit = DIVIDER/size;
	for (int k = 0; k < limit; k++){
		//insertion sort algorithm
		int i, j;
		int key, index;

		for(j = 1 ; j < size ; j++){
			key = matrix[j][1];
			index = matrix[j][0];
			i = j - 1;
			while(i >= 0 && matrix[i][1] > key){
				matrix[i+1][1] = matrix[i][1];
				matrix[i+1][0] = matrix[i][0];
				i--;
			}
			matrix[i+1][1] = key;
			matrix[i+1][0] = index;
			//sorting is done
			
			//reset the matrix for re-sorting
			if(k != limit - 1){
				for(int a = 0 ; a < size ; a++){
					matrix[a][0] = original_matrix[a][0];
					matrix[a][1] = original_matrix[a][1];
				}
			}
		}

	}
	int end_time = clock();
	
	//running time algorithm is defined in the above
	//print the running time
	cout << fixed;
	cout << setprecision(20) << (double)(end_time - start_time)/limit << " ms" << endl; 
	
	return;
}
