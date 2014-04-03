/* 
 * File:   PascalTriangle.cpp
 * Author: Volkan İlbeyli
 * 
 * Created on April 27, 2013, 8:01 PM
 */

#include "PascalTriangle.h"
#include <iostream>
#include <cstdio>

//constructors & destructors
PascalTriangle::PascalTriangle() {
}
PascalTriangle::PascalTriangle(const PascalTriangle& orig) {
}
PascalTriangle::~PascalTriangle() {
    //since a vector is used, no need for deallocation or anything
    //garbage collection of STL handles things
}
PascalTriangle::PascalTriangle(const unsigned int n, const unsigned int k){
    // n>=k is checked from the main function, thus it is guaranteed that n>=k
    
    //variables for initializing the triangle
    BigUnsignedInteger num; //number for populating vectors
    vector<BigUnsignedInteger> vec; //vectors for populating triangle rows
    num =0;     //initially values are 0 (addition identity element)
    vec.push_back(num);
    
    // push 0 valued big unsigned integer lists to triangle
    for(int i=0 ; i<=n ; i++){
        triangle.push_back(vec);    //0 valued vectors are pushed back
        vec.push_back(num);     //every row triangle increases in size
    }
    
    //build the triangle up
    for(int i=0; i<=n ; i++){   //for each row
        triangle[i][0] = 1; //set first and last elements to 1
        triangle[i][i] = 1;
        for(int j=1; j<i ; j++){
            //no need for optimization for the if block below 
            //unless branch prediction is disabled 
            if(i!=0){ 
                triangle[i][j] += triangle[i-1][j] + triangle[i-1][j-1];
            }
        }
    }
    
    //triangle ready. write to file
    FILE* fp;
    fp = fopen("pascal.txt","w");
    if(fp == NULL){ //error check
        cout << "Error opening file: pascal.txt" << endl;
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
void PascalTriangle::print() const{
    for(int i=0; i<triangle.size() ; i++){
        for(int j=0 ; j<triangle[i].size() ; j++){
            triangle[i][j].print();
        }
        cout << endl;
    }
}
