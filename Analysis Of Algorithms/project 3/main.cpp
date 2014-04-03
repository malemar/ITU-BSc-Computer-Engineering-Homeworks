/****************************************/
/*	BLG372E - Analysis of Algorithms	*/
/*				Spring 2013				*/
/*				Project III				*/
/*                                      */
/*			Volkan İlbeyli				*/
/*			   040100118                */
/*										*/
/*		Instructor: Hazım Ekenel        */
/****************************************/

#include <iostream>
#include <cstdlib>

#include "BigUnsignedInteger.h"
#include "PascalTriangle.h"
#include "CombinatorialSolution.h"

using namespace std;

#define LIMIT 4294967295

//function prototypes


int main(int argc, char** argv) {
    //main parameter check
    if (argc != 3) {
        cout << "Invalid argument count." << endl;
        cout << "Example: ./a.out 7 4" << endl;
        cout << "Returning from main()..." << endl;
        return -1;
    }
    unsigned int n = atoi(argv[1]);
    unsigned int k = atoi(argv[2]);
    if(n < k){
        cout << "n can't be smaller than k" << endl;
        cout << "Returning from main()..." << endl;
        return -1;  
    }

//    //ARITHMETIC TESTS
//    BigUnsignedInteger n1;
//    BigUnsignedInteger n2;
//    BigUnsignedInteger n3;
//    BigUnsignedInteger n4;
//    BigUnsignedInteger n5;
//    BigUnsignedInteger n6;
//    n1 += LIMIT;
//    n1 += 3067833788 +1;
//    n1.print();
//    n2 += LIMIT/5;
//    n1 /= n2;
//    n1.print();
//    
//    n3 += 3067833788;
//    n4 += 2576980376;
//    n3 = n3 - n4;
//    n3.print();
//    n4 = n4 >> 1;
//    n4.print();
//    n5 += 5;
//    n6 += 5;
//    n5.print(); n6.print();
//    n5 -= n6;
//    n5.print();
//    if(n1 < n2) cout << "hallelujah" << endl;
//    if(n1 < LIMIT) cout << "hallelujah" << endl;
    /*  number * 2^22 = 8s
     *  number << 22 = 200 ms
     */
    
//    //LIMIT TESTS
//    unsigned long long a = (LIMIT+1)*(LIMIT+1)-1;
//    unsigned long long b = (LIMIT*LIMIT);
//    cout << b << endl;
//    cout << a << endl;
//    int length=0;
//    while(a!=0){
//        a=a>>1;
//        length++;
//    }
//    cout << "length a: " << length << endl;
//    length=0;
//    while(b!=0){
//        b=b>>1;
//        length++;
//    }
//    cout << "lenght b: " << length << endl;

    //ALGORITHM TESTS
    cout << "************************" << endl;
    //PascalTriangle p(n,k);
    //p.print();    //prints triangle
    cout << "------------------------" << endl;
    CombinatorialSolution c(n,k);
    c.print();    //prints triangle
    
//    char ch;
//    cin >> ch;
    return 0;
}

