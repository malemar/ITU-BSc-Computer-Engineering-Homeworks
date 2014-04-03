/* 
 * File:   CombinatorialSolution.cpp
 * Author: Volkan İlbeyli
 * 
 * Created on April 28, 2013, 12:57 AM
 */

#include "CombinatorialSolution.h"
#include <cstdio>
#include <iostream>

//constructors & destructors
CombinatorialSolution::CombinatorialSolution() {
}
CombinatorialSolution::CombinatorialSolution(const CombinatorialSolution& orig) {
}
CombinatorialSolution::~CombinatorialSolution() {
}
CombinatorialSolution::CombinatorialSolution(unsigned const int n, unsigned const int k){
    // n>=k is checked from the main function, thus it is guaranteed that n>=k
    //in this solution, not all factorial values are calculated.
    //                         5 * 4
    //since, e.g., C(5,2) = -----------
    //                          2!
    //therefore, less multiplication operations are made this way.
    
    //variables for initializing the triangle
    BigUnsignedInteger num; //number for populating vectors
    vector<BigUnsignedInteger> vec; //vectors for populating triangle rows
    num =1;     //initially values are 1 (multiplication identity element)
    vec.push_back(num);
    
    // push 0 valued big unsigned integer lists to triangle
    for(int i=0 ; i<=n ; i++){
        triangle.push_back(vec);    //0 valued vectors are pushed back
        vec.push_back(num);     //every row triangle increases in size
    }
    
    //build the triangle up
    //FOR SOME REASON, i WONT BE 21???? 
    for(int i=0; i<=n ; i++){   //for each row of triangle
		if (i == 13)
		{
			cout << "asd";
		}
        cout << i << ") ";
        for(int j=1; j<=i-1 ; j++){    //combination counter
            cout << j << " ";
            BigUnsignedInteger nom;     nom = 1;    //nominator of combination
            BigUnsignedInteger denom; denom = 1;  //denominator of combination
            
            //no need for optimization for the if block below 
            //unless branch prediction is disabled 
            if(i!=0){ 
                for(int k=0 ; k<j ; k++){ //j times
                    //multiply nominator, by decrementing in each iteration by k
                    //thus nominator = n!/(n-k)!
                    nom *= (i-k);
                    
                    //multiply denominator by k+1, incrementing each iteration
                    //thus denominator = j!
                    denom *= k+1;
                }
//                cout << "triangle["<<i<<"]["<<j<<"]=" << endl;
//                nom.print(); denom.print(); 
//                if(j >= 18) cout << " j="<<j<<" ";
                triangle[i][j] = nom/denom;
//                cout << "division done" << endl;
//                cout << "comb*result="; triangle[i][j].print(); cout << endl;
            }
        }
//        cout << "  DONE, next i=" << i+1;
        cout << endl;
    }
    
    //triangle ready. write to file
    FILE* fp;
    fp = fopen("combinatorial.txt","w");
    if(fp == NULL){ //error check
        cout << "Error opening file: combinatorial.txt" << endl;
        cout << "Press Enter to continue. . ." << endl;
        getchar();
    }
    else{   //no error
        fprintf(fp,"n k length\n");     //first row of file
        for(int i=0; i<=n ; i++){   //triangle rows
            for(int j=0; j<=i ; j++){   //row elements
                if(i==n && j==k)   //row containing the desired value
                    fprintf(fp, "%d %d is equal to %llu\n",i,j,triangle[i][j].val());
                else    //others are lengths
                    fprintf(fp,"%d %d %d\n", i,j, triangle[i][j].length());
            }
        }
    }
    fclose(fp);
}

//member functions
void CombinatorialSolution::print() const{
    for(int i=0; i<triangle.size() ; i++){
        for(int j=0 ; j<triangle[i].size() ; j++){
            triangle[i][j].print();
        }
        cout << endl;
    }
}