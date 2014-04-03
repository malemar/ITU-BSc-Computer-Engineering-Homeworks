/*
*
* BLG252E
* 2012 Spring
* 3rd Homework
*
*/
/*
* @Author
* Student Name: Volkan Ilbeyli
* Student ID : 040100118
* Date: 09.05.2012
*/
#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;

template <class T>
class Matrix{
public:
	Matrix(const int, const int);
	~Matrix();
	Matrix(const Matrix&);
	const int get_col() const { return col; }
	const int get_row() const { return row; }
	T** get_matrice() const { return &matrice[0]; }
	T getElement(const int, const int) const;
	void print() const;
	bool contains(const T) const;
	const Matrix& operator/(const Matrix&);
	const Matrix& operator+(const Matrix&);
	const Matrix& operator=(const Matrix&);
	void operator--();
	void operator++();
	bool operator==(const Matrix&);
private:
	T** matrice;
	const int row,col;

};

//Constructors
template <class T>
Matrix<T>::Matrix(const int rowIn, const int colIn) : row(rowIn), col(colIn){

	//memory allocation for 2D matrice
	matrice = new T*[row];
	for(int i=0 ; i < row ; i++)
		matrice[i] = new T[col];

	//value initialization of matrice
	for(int i=0 ; i < row ; i++){
		for(int j=0 ; j < col ; j++){
			matrice[i][j] = rand()%11;
		}
	}
	
}

template <class T>
Matrix<T>::~Matrix(){
	for(int i = 0 ; i < row ; i++)
		delete[] matrice[i];
	delete[] matrice;
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& objIn) : row(objIn.get_row()), col(objIn.get_col()) {

	//memory allocation
	matrice = new T*[row];
	for(int i=0 ; i < row ; i++)
		matrice[i] = new T[col];

	//member initialization
	for(int i=0 ; i < row ; i++){
		for(int j=0 ; j < col ; j++){
			matrice[i][j] = objIn.matrice[i][j];
		}
	}
}


//Member functions
template <class T>
void Matrix<T>::print() const{
	for(int i=0 ; i < row ; i++){
		cout << "Row " << i+1 << ": ";
		for(int j=0 ; j < col ; j++){
			cout << matrice[i][j] << '\t';
		}
		cout << endl;
	}
}

template <class T>
T Matrix<T>::getElement(const int rowIn, const int colIn) const{
	if(rowIn > row || colIn > col) throw string("Error: getElement() : Request out of matrice bounds.");
	else
		return matrice[(rowIn-1)][colIn-1];		//assuming the indexes specified in main 
												//are through 1 to upper limit, not from 0.
}

template <class T>
bool Matrix<T>::contains(const T check) const{
	for(int i=0 ; i < row ; i++){
		for(int j=0 ; j < col ; j++){
			if(check == matrice[i][j])
				return true;
		}
	}
	return false;
}


//Operators
template <class T>
const Matrix<T>& Matrix<T>::operator/(const Matrix<T>& objIn){
	if(objIn.get_row() != row || objIn.get_col() != col) //unsuitable dimension condition
		throw string("Error: Matrice dimensions are not suitable for division operation.");
	else{
		T** inMatrice = objIn.get_matrice();
		for(int i=0 ; i < row ; i++){
			for(int j=0 ; j < col ; j++){
				matrice[i][j] /= inMatrice[i][j];
			}
		}
		return *this;
	}
}

template <class T>
const Matrix<T>& Matrix<T>::operator+(const Matrix<T>& objIn){
	if(objIn.get_row() != row || objIn.get_col() != col) //unsuitable dimension condition
		throw string("Error: Matrice dimensions are not suitable for addition operation.");
	else{
		T** inMatrice = objIn.get_matrice();
		for(int i=0 ; i < row ; i++){
			for(int j=0 ; j < col ; j++){
				matrice[i][j] += inMatrice[i][j];
			}
		}
		return *this;
	}
}

template <class T>
const Matrix<T>& Matrix<T>::operator=(const Matrix<T>& objIn){
	if(objIn.get_row() != row || objIn.get_col() != col) //unsuitable dimension condition
		throw string("Error: Matrice dimensions are not suitable for assignment operation.");
	else{
		T** inMatrice = objIn.get_matrice();
		for(int i=0 ; i < row ; i++){
			for(int j=0 ; j < col ; j++){
				matrice[i][j] = inMatrice[i][j];
			}
		}
		return *this;
	}
}

template <class T>
void Matrix<T>::operator--(){
	for(int i=0 ; i < row ; i++){
		for(int j=0 ; j < col ; j++){
			matrice[i][j]--;
		}
	}
}

template <class T>
void Matrix<T>::operator++(){
	for(int i=0 ; i < row ; i++){
		for(int j=0 ; j < col ; j++){
			matrice[i][j]++;
		}
	}	
}

template <class T>
bool Matrix<T>::operator==(const Matrix<T>& objIn){
	if(objIn.get_row() != row || objIn.get_col() != col) //unsuitable dimension condition
		throw string("Error: Matrice dimensions are not suitable for comparison.");
	else{
		bool is_equal = true;	//flag
		T** inMatrice = objIn.get_matrice();
		for(int i=0 ; i < row ; i++){
			for(int j=0 ; j < col ; j++){
				if(matrice[i][j] != inMatrice[i][j])
					is_equal = false;
			}
		}
		return is_equal;
	}
}


int main()
{
	srand(time(NULL));
	Matrix<int> m1(3,5);  // creating some objects 
	Matrix<int> m2(3,5);  // matrices’ elements are assigned randomly from 0 to 10
	Matrix<double> m3(5,5);
	Matrix<double> m4(5,6);

	try{
		cout << m1.getElement(3,6) << endl;   // trying to get the element at(3,6)
	}
	catch(const string & err_msg)
	{ 
		cout << err_msg << endl;   
	}

	cout << "Printing m4" << endl;
	m4.print();                              // printing m4

	try{ 
		Matrix<double> m6 = m4 / m3;        // trying to divide two matrices
	}
	catch(const string & err_msg)
	{ 
		cout << err_msg << endl;   
	}

	cout << "Printing m1" << endl;
	m1.print();

	if(m1.contains(4))     // checking if the matrix has an element with value 4
		cout << "Matrix contains the element" << endl;
	else
		cout << "Matrix does not contain the element" << endl;

	Matrix<int> m5 = m2;
	cout << "Printing m5" << endl;
	m5.print();

	try{
		--m1;           // decrement m1's matrix elements by 1
		m5 = m1 + m2;   // sum m1 and m2 object's matrices and assign result to m5
		++m3;           // increment m1's matrix elements by 1
		m2 = m5 / m1;   // divide m5 and m1 object's matrices and assign result to m2
		if(m5 == m1)    // comparing two objects
			cout << "Objects are equal" << endl;
		else
			cout << "Objects are not equal" << endl;
	}
	catch(const string & err_msg)
	{ 
		cout << err_msg << endl;   
	}

	cout << "Printing m5" << endl;
	m5.print();
}

