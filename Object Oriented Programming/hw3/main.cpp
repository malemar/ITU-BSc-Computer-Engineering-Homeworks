#include <iostream>
#include <time.h>
#include <string>


using namespace std;

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
