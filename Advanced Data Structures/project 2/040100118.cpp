/****************************************/
/*	BLG381E - Advanced Data Structures	*/
/*				Fall 2012				*/
/*										*/
/*			Volkan Ilbeyli				*/
/*			   040100118				*/
/*										*/
/*		Insturctor: Zehra Cataltepe		*/
/****************************************/
/*				NOT: 102				*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

//heapsort prototypes
template <class T>
void max_heapify(T*, int, int);

template <class T>
void build_max_heap(T*, int);

template <class T>
void heapsort(T*, int);

//priority queue prototypes
template <class T>
T maximum(T*);

template <class T>
T extract_max(T*, int);

template <class T>
void increase_key(T*, int, T);

template <class T>
void insert(T*, T, int);

///given sort algorithms
template <class T>
void insertion_sort(T *array, int size){
    T key;
    int i, j;
    for(j=1; j<=size; j++){
        key = array[j];
        i = j-1;
        while(i>=0 && array[i]>key){
            array[i+1] = array[i];
            i--;
        }
        array[i+1] = key;
    }
}

template <class T>
void merge(T *A, int p, int q, int r){
    int n1=q-p+1, n2=r-q;
    T L[n1+1], R[n2+1];
    int i,j,k;
    for(i=0; i<n1; i++)
        L[i] = A[p+i];
    for(i=0; i<n2; i++)
        R[i] = A[q+i+1];
    i = j = 0;
    for(k=p; k<=r; k++){
        if (i==n1){
            A[k] = R[j];
            j++;
        }
        else if (j==n2){
            A[k] = L[i];
            i++;
        }
        else if (L[i] <= R[j]){
            A[k] = L[i];
            i++;
        }
        else{
            A[k] = R[j];
            j++;
        }
    }
}

template <class T>
void merge_sort(T *A, int p, int r){
    int q;
    if (p < r){
        q = (int)floor((p+r)/2);
        merge_sort(A, p, q);
        merge_sort(A, q+1, r);
        merge(A, p, q, r);
    }
}

int main(){
    srand((unsigned)time(NULL));
	const int MAX_SIZE = 100;
	const int SIZE = 10;

	int a1[MAX_SIZE] = {0}; 
	int a2[MAX_SIZE] = {0};
    cout << "Unsorted array: " << endl;
    for (int i=0; i<SIZE; i++){
        a1[i] = rand()%100;
        a2[i] = a1[i];
        cout << a1[i] << "\t";
    }
	/*
    cout << endl << "Insertion sort: ";
    insertion_sort(a1, 9);;
    for (int i=0; i<10; i++)
        cout << a1[i] << " ";

    cout << endl <<"Merge sort: ";
    merge_sort(a2, 0, 9);
    for (int i=0; i<10; i++)
        cout << a2[i] << " ";
	*/

	cout << endl << "heapsort(a1, SIZE-1); " << endl;
	heapsort(a1, SIZE-1);	//parameter size is the upper bound of the array
    for (int i=0; i<SIZE; i++)
        cout << a1[i] << "\t";

	cout << endl << "increase_key(a1, 0, 103); " << endl;
	increase_key(a1, 0, 103); 
	for (int i=0; i<SIZE; i++)
        cout << a1[i] << "\t";

	cout << endl << "insert(a1, 32, SIZE-1); " << endl;
	insert(a1, 32, SIZE-1);
	for (int i=0; i<SIZE+1; i++)
        cout << a1[i] << "\t";

	cout << endl << endl << "build_max_heap(a2, SIZE-1);" << endl;
	build_max_heap(a2, SIZE-1);
	for (int i=0; i<SIZE; i++)
        cout << a2[i] << "\t";

	cout << endl << "extract_max(a2, SIZE-1) " << endl;
	cout << "extracted value -->" << extract_max(a2, SIZE-1) << endl;
	cout << endl << "print" << endl;
	for (int i=0; i<SIZE; i++)
        cout << a2[i] << "\t";
	
	cout << endl << "**************************" << endl;
	cout << "be patient! it WILL show results in around 15 seconds. calculating now. . ." << endl;

	int heap_runtime[7];
	int insertion_runtime[7];
	int merge_runtime[7];
	
	/*
			IMPORTANT NOTE: Ignore the comments near MERGESORT and 
			INSERTION SORT runtime calculating codes. Even though
			the code you will see below this comment is redundant, it 
			runs fine, just hard to read. 
			(akilsiz basin cezasini parmaklar cekti. . .)
	*/
	//********************************************************** 10 INPUT
	const int size1 = 10;
	int arr1[size1], arr12[size1];
	int start_time, end_time, runtime=0;

	for(int i = 0 ; i < size1 ; i++){
		arr1[i] = rand()%100;
		arr12[i] = arr1[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10000 ; k++){	//run 5000 times, 10 input
			heapsort(arr1, size1-1);
			for(int i = 0 ; i < size1 ; i++)
				arr1[i] = arr12[i];		//reset array
		}
		end_time = clock();
		runtime += (end_time - start_time)/10000;
		for(int i = 0 ; i < size1 ; i++){
			arr1[i] = rand()%100;
			arr12[i] = arr1[i];
		}
	}
	heap_runtime[0] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			merge_sort(arr1, 0, size1-1);
			for(int i = 0 ; i < size1 ; i++)
				arr1[i] = arr12[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size1 ; i++){
			arr1[i] = rand()%100;
			arr12[i] = arr1[i];
		}
	}
	merge_runtime[0] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr1, size1-1);
			for(int i = 0 ; i < size1 ; i++)
				arr1[i] = arr12[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size1 ; i++){
			arr1[i] = rand()%100;
			arr12[i] = arr1[i];
		}
	}
	insertion_runtime[0] = runtime/10;


	//********************************************************** 20 INPUT
	const int size2 = 20;
	int arr2[size2], arr22[size2];
	runtime = 0;

	for(int i = 0 ; i < size2 ; i++){
		arr2[i] = rand()%100;
		arr22[i] = arr2[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<5000 ; k++){	//run 5000 times, 20 input
			heapsort(arr2, size2-1);
			for(int i = 0 ; i < size2 ; i++)
				arr2[i] = arr22[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/5000;
		for(int i = 0 ; i < size2 ; i++){
			arr2[i] = rand()%100;
			arr22[i] = arr2[i];
		}
	}
	heap_runtime[1] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			merge_sort(arr2, 0, size2-1);
			for(int i = 0 ; i < size2 ; i++)
				arr2[i] = arr22[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size2 ; i++){
			arr2[i] = rand()%100;
			arr22[i] = arr2[i];
		}
	}
	merge_runtime[1] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr2, size2-1);
			for(int i = 0 ; i < size2 ; i++)
				arr2[i] = arr22[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size2 ; i++){
			arr2[i] = rand()%100;
			arr22[i] = arr2[i];
		}	
	}
	insertion_runtime[1] = runtime/10;


	//********************************************************** 50 INPUT
	const int size3 = 50;
	int arr3[size3], arr32[size3];
	runtime = 0;

	for(int i = 0 ; i < size3 ; i++){
		arr3[i] = rand()%100;
		arr32[i] = arr3[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<5000 ; k++){	//run 5000 times, 50 input
			heapsort(arr3, size3-1);
			for(int i = 0 ; i < size3 ; i++)
				arr3[i] = arr32[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/5000;
		for(int i = 0 ; i < size3 ; i++){
			arr3[i] = rand()%100;
			arr32[i] = arr3[i];
		}
	}
	heap_runtime[2] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			merge_sort(arr3, 0, size3-1);
			for(int i = 0 ; i < size3 ; i++)
				arr3[i] = arr32[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size3 ; i++){
			arr3[i] = rand()%100;
			arr32[i] = arr3[i];
		}
	}
	merge_runtime[2] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr3, size3-1);
			for(int i = 0 ; i < size3 ; i++)
				arr3[i] = arr32[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size3 ; i++){
			arr3[i] = rand()%100;
			arr32[i] = arr3[i];
		}
	}
	insertion_runtime[2] = runtime/10;


	//********************************************************** 100 INPUT
	const int size4 = 100;
	int arr4[size4], arr42[size4];
	runtime = 0;

	for(int i = 0 ; i < size4 ; i++){
		arr4[i] = rand()%100;
		arr42[i] = arr4[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<5000 ; k++){	//run 5000 times, 100 input
			heapsort(arr4, size4-1);
			for(int i = 0 ; i < size4 ; i++)
				arr4[i] = arr42[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/5000;
		for(int i = 0 ; i < size4 ; i++){
			arr4[i] = rand()%100;
			arr42[i] = arr4[i];
		}
	}
	heap_runtime[3] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			merge_sort(arr4, 0, size4-1);
			for(int i = 0 ; i < size4 ; i++)
				arr4[i] = arr42[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size4 ; i++){
			arr4[i] = rand()%100;
			arr42[i] = arr4[i];
		}
	}
	merge_runtime[3] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr4, size4-1);
			for(int i = 0 ; i < size4 ; i++)
				arr4[i] = arr42[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size4 ; i++){
			arr4[i] = rand()%100;
			arr42[i] = arr4[i];
		}
	}
	insertion_runtime[3] = runtime/10;


	//********************************************************** 1000 INPUT
	const int size5 = 1000;
	int arr5[size5], arr52[size5];
	runtime = 0;

	for(int i = 0 ; i < size5 ; i++){
		arr5[i] = rand()%100;
		arr52[i] = arr5[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<1000 ; k++){	//run 100 times, 1000 input
			heapsort(arr5, size5-1);
			for(int i = 0 ; i < size5 ; i++)
				arr5[i] = arr52[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/1000;
		for(int i = 0 ; i < size5 ; i++){
			arr5[i] = rand()%100;
			arr52[i] = arr5[i];
		}
	}
	heap_runtime[4] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			merge_sort(arr5, 0, size5-1);
			for(int i = 0 ; i < size5 ; i++)
				arr5[i] = arr52[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size5 ; i++){
			arr5[i] = rand()%100;
			arr52[i] = arr5[i];
		}
	}
	merge_runtime[4] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<100 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr5, size5-1);
			for(int i = 0 ; i < size5 ; i++)
				arr5[i] = arr52[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/100;
		for(int i = 0 ; i < size5 ; i++){
			arr5[i] = rand()%100;
			arr52[i] = arr5[i];
		}
	}
	insertion_runtime[4] = runtime/10;


	//********************************************************** 5000 INPUT
	const int size6 = 5000;
	int arr6[size6], arr62[size6];
	runtime = 0;

	for(int i = 0 ; i < size6 ; i++){
		arr6[i] = rand()%100;
		arr62[i] = arr6[i];
	}

	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 5000 input
			heapsort(arr6, size6-1);
			for(int i = 0 ; i < size6 ; i++)
				arr6[i] = arr62[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		for(int i = 0 ; i < size6 ; i++){
			arr6[i] = rand()%100;
			arr62[i] = arr6[i];
		}
	}
	heap_runtime[5] = runtime/10;
	
	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 10000 input
			merge_sort(arr6, 0, size6-1);
			for(int i = 0 ; i < size6 ; i++)
				arr6[i] = arr62[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		for(int i = 0 ; i < size6 ; i++){
			arr6[i] = rand()%100;
			arr62[i] = arr6[i];
		}
	}
	merge_runtime[5] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr6, size6-1);
			for(int i = 0 ; i < size6 ; i++)
				arr6[i] = arr62[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		for(int i = 0 ; i < size6 ; i++){
			arr6[i] = rand()%100;
			arr62[i] = arr6[i];
		}
	}
	insertion_runtime[5] = runtime/10;

	
	//********************************************************** 10000 INPUT
	const int size7 = 10000;
	int arr7[size7], arr72[size7];
	runtime = 0;

	//initialize array
	for(int i = 0 ; i < size7 ; i++){
		arr7[i] = rand()%100;
		arr72[i] = arr7[i];
	}

	//HEAPSORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 10000 input
			heapsort(arr7, size7-1);
			for(int i = 0 ; i < size7 ; i++)
				arr7[i] = arr72[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		
		//initialize array
		for(int i = 0 ; i < size7 ; i++){
			arr7[i] = rand()%100;
			arr72[i] = arr7[i];
		}
	}
	heap_runtime[6] = runtime/10;

	//MERGESORT 
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 10000 input
			merge_sort(arr7, 0, size7-1);
			for(int i = 0 ; i < size7 ; i++)
				arr7[i] = arr72[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		//initialize array
		for(int i = 0 ; i < size7 ; i++){
			arr7[i] = rand()%100;
			arr72[i] = arr7[i];
		}
	}
	merge_runtime[6] = runtime/10;

	//INSERTION SORT
	for(int j = 0 ; j < 10 ; j++){		//run 10 times
		start_time = clock();
		for(int k = 0 ; k<10 ; k++){	//run 100 times, 10000 input
			insertion_sort(arr7, size7-1);
			for(int i = 0 ; i < size7 ; i++)
				arr7[i] = arr72[i];
		}
		end_time = clock();
		runtime += (end_time - start_time)/10;
		//initialize array
		for(int i = 0 ; i < size7 ; i++){
			arr7[i] = rand()%100;
			arr72[i] = arr7[i];
		}
	}
	insertion_runtime[6] = runtime/10;

	// ******* print running times
	for(int i = 0 ; i<7 ; i++){
		if(i >= 4)
			cout << "heapsort: " << heap_runtime[i] <<" ms\tmerge sort: " << merge_runtime[i] << " ms\tinsertion sort: " << insertion_runtime[i] << " ms" << endl;
		else
			cout << "heapsort: " << heap_runtime[i] <<" ms\t\tmerge sort: " << merge_runtime[i] << " ms\tinsertion sort: " << insertion_runtime[i] << " ms" << endl;
	}
    return 0;
}

//HEAPSORT FUNCTIONS

template <class T>
void max_heapify(T* A, int i, int size){
	int l = 2*i + 1;
	int r = 2*i + 2;
	T largest, temp;

	if(l <= size && A[l] > A[i])
		largest = l;
	else
		largest = i;

	if(r <= size && A[r] > A[largest])
		largest = r;

	if(largest != i){
		temp = A[i];
		A[i] = A[largest];
		A[largest] = temp;
		max_heapify(A, largest, size);
	}

	return;
}

template <class T>
void build_max_heap(T* A, int heapsize){
	for(int i = heapsize/2 ; i >= 0 ; i--)
		max_heapify(A, i, heapsize);
	return;
}

template <class T>
void heapsort(T* A, int size){
	T tmp;

	build_max_heap(A, size);
	for(int i = size ; i >= 1 ; i--){
		tmp = A[0];
		A[0] = A[i];
		A[i] = tmp;
		size--;
		max_heapify(A, 0, size);
	}
}


//PRIORITY QUEUE FUNCTIONS

template <class T>
T maximum(T* A){ return A[0]; }

template <class T>
T extract_max(T* A, int size){
	T max;

	if(size < 1)
		cout << "heapsize underflow: size = " << size << endl;
	max = A[0];
	A[0] = A[size];	//last element to root
	size--;
	max_heapify(A,0,size);
	return max;
}

template <class T>
void increase_key(T* A, int i, T key){
	int parent = (i-1)/2;
	T tmp;

	if(A[i] > key){
		cout << "new key = " << key << " is smaller than current key A[i] = " << A[i] << endl;
		return;
	}

	A[i] = key;
	while(i > 0 && A[parent] < A[i]){
		tmp = A[i];
		A[i] = A[parent];
		A[parent] = tmp;
		i = parent;
	}

	return;
}

template <class T>
void insert(T* A, T key, int size){
	//this function only executes IFF the array is not full
	//i.e. size != max_size. you have to define an array
	// A[MAX_SIZE] and when passing to functions, you should use
	// SIZE - 1, just like when you are assigning random numbers to it,
	//in for loop as for(int i = 0 ; i < SIZE ; i++)
	//remember: you should pass SIZE-1 to functions, i.e. upper bound of array.

	size++;
	//A[size] = -9999;
	increase_key(A, size, key);
}
