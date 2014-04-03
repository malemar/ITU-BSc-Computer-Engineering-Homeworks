/* 
 * File:   BigUnsignedInteger.cpp
 * Author: Volkan İlbeyli
 * 
 * Created on April 25, 2013, 10:14 PM
 */

#include "BigUnsignedInteger.h"
#include <iostream>

using namespace std;

#define LIMIT 4294967295
#define OPT
#define DIV

//STL Macros
typedef vector<unsigned int> vui;

#define Iterate(it,c) for(vui::iterator it = c.begin() ; it != c.end() ; it++)
#define rIterate(it,c) for(vui::const_iterator it = c.end()-1 ; it != c.begin()-1 ; it--)


//constructors & destructors
BigUnsignedInteger::BigUnsignedInteger() {
    value.push_back(0);
}
BigUnsignedInteger::BigUnsignedInteger(const BigUnsignedInteger& orig) {
    value = orig.getValue();
}
BigUnsignedInteger::~BigUnsignedInteger() {
    //since a vector is used, no need for deallocation or anything
    //garbage collection of STL handles things
}

//member functions
void BigUnsignedInteger::print() const {
    rIterate(it,value){
        cout << *it << " ";
    }
    cout << "  length = " << this->length() << endl;
}
unsigned int BigUnsignedInteger::length() const{
    unsigned int length = 32 * (value.size()-1);
    unsigned int number = value.back();
    while(number != 0){
        number = number >> 1;
        length++;
    }
    if(number == 0 && length == 0)
        return 1;
    else
        return length;
}
unsigned long long BigUnsignedInteger::val() const{
    unsigned long long decimal = 0;
    for(int i=0; i<value.size() ; i++){
        decimal += (LIMIT*i+1)*value[i];
    }
    return decimal;
}

//operators
BigUnsignedInteger BigUnsignedInteger::operator =(const unsigned int& number){
    value.clear();
    value.push_back(number);
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator +(const BigUnsignedInteger& number){
    BigUnsignedInteger dummyInt(*this);
    vui val = dummyInt.getValue();      //temporary container to work on
    
    //ready the operands, make #digits equal
    if(number.getValue().size() != val.size()){   //if #digits not equal
        if(val.size() < number.getValue().size()){  //if rhs has more digits
            int difference = number.getValue().size() - val.size();
            for(int i=0 ; i<difference ; i++)
                val.push_back(0);   //make lhs equal to rhs, digit-wise
        }                           
    }
    
    //add the big guys up
    int i;   //loop variables
    unsigned long long dummy;   //carry control variable
    unsigned int carry = 0;
    for(i=0 ; i<val.size() ; i++){
        dummy = val[i];
        dummy += number.getValue()[i];
        dummy += carry;
        if(dummy > LIMIT){  //if carry
            val[i] = val[i] - ((LIMIT - number.getValue()[i])+1+carry);
            carry = 1;
        }
        else{   //if no carry
            val[i] += number.getValue()[i];
            carry = 0;
        }
    }
    if(carry == 1)  //if carry at the end of the addition
        val.push_back(1);   //add extra digit
    
    dummyInt.setValue(val);
    return dummyInt;
}
BigUnsignedInteger BigUnsignedInteger::operator +=(const BigUnsignedInteger& number){
    BigUnsignedInteger dummyInt(*this);
    vui val = dummyInt.getValue();      //temporary container to work on
    
    //ready the operands, make #digits equal
    if(number.getValue().size() != val.size()){   //if #digits not equal
        if(val.size() < number.getValue().size()){  //if rhs has more digits
            int difference = number.getValue().size() - val.size();
            for(int i=0 ; i<difference ; i++)
                val.push_back(0);   //make lhs equal to rhs, digit-wise
        }                           
        
    }
    
    //add the big guys up
    int i;   //loop variables
    unsigned long long dummy;   //carry control variable
    unsigned int carry = 0;
    for(i=0 ; i<val.size() ; i++){
        dummy = val[i];
        dummy += number.getValue()[i];
        dummy += carry;
        if(dummy > LIMIT){  //if carry
            val[i] = val[i] - ((LIMIT - number.getValue()[i])+1);
            carry = 1;
        }
        else{   //if no carry
            val[i] += carry;
            val[i] += number.getValue()[i];
            carry = 0;
        }
    }
    if(carry == 1)  //if carry at the end of the addition
        val.push_back(1);   //add extra digit
    
    dummyInt.setValue(val);
    *this = dummyInt;
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator +=(const unsigned int& number){
    unsigned long long dummy;   // an int with larger limit than unsigned int 
    dummy = value[0];
    dummy += number;  // for checking overflow
    //cout << "dummy = " << dummy << ", number = " << number << endl;
    if(dummy > LIMIT){  //if carry
        //cout << "+ carry!!" << endl;
        if(value.size() == 1){  //if value has 1 digit
            value.push_back(1);  // add 1 more digit with value 1
            value[0] = value[0] - ((LIMIT - number)+1); // update least
        }                                               // significant digit
        else{       //if size is greater than 1
            int i;
            for(i=1 ; i<value.size() ; i++){  //iterate for carry
                dummy = value[i]+1; //carry added to next digit
                if(dummy > LIMIT){  //overflow control
                    value[i] = 0;   //reset digit (carry + limit = 0)
                    continue;       //move to next digit
                }
                else{           //if no overflow
                    value[i]++; //increment digit
                    break;      //stop iteration
                }
            }   //carry operation done
            value[0] = value[0] - ((LIMIT - number)+1); // update last digit
            if(i == value.size())  //if most significant digit overflows too
                value.push_back(1); //add 1 more digit to value
        }
    }   //end if(overflow)
    else   //if no overflow occurs
        value[0] += number;
    
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator +(const unsigned int& number){
    unsigned long long dummy;   // an int with larger limit than unsigned int
    BigUnsignedInteger dumb(*this);
    vui val = dumb.getValue(); //temporary container
    dummy = val[0];
    dummy += number;  // for checking overflow
    if(dummy > LIMIT){  //if overflow occurs
        if(val.size() == 1){  //if value has 1 digit
            val.push_back(1);  // add 1 more digit with value 1
            val[0] = val[0] - ((LIMIT - number)+1); // update least
        }                                               // significant digit
        else{
            int i;
            for(i=1 ; i<val.size() ; i++){  //iterate for carry
                dummy = val[i]+1; //carry added to next digit
                if(dummy > LIMIT){  //overflow control
                    val[i+1] = 0; //reset digit
                    continue;       //move to next digit
                }
                else{           //if no overflow
                    val[i]++; //increment digit
                    break;      //stop iteration
                }
            }
            value[0] = value[0] - ((LIMIT - number)+1); // update last digit
            if(i == val.size())  //if last digit overflows too
                val.push_back(1); //add 1 more digit to value
        }
    }   //end if(overflow)
    else   //if no overflow occurs
        val[0] += number;
    dumb.setValue(val); //set the value
    return dumb;
}
BigUnsignedInteger BigUnsignedInteger::operator *(const BigUnsignedInteger& number){
    vector<BigUnsignedInteger> midResults;
    BigUnsignedInteger result;
    BigUnsignedInteger dummyInt(*this);
    vui val = dummyInt.getValue();      //temporary container to work on
    //ready the operands, make #digits equal
    if(number.getValue().size() != val.size()){   //if #digits not equal
        if(val.size() < number.getValue().size()){  //if rhs has more digits
            int difference = number.getValue().size() - val.size();
            for(int i=0 ; i<difference ; i++)
                val.push_back(0);   //make lhs equal to rhs, digit-wise
        }                           
    }
    
    //get the mid-results of multiplication
    for(int i=0; i<val.size() ; i++){
        result = dummyInt*number.getValue()[i];
        for(int j=0 ; j<i ; j++){   //OPTIMIZED if defined OPT
            result = result*LIMIT;
        }
        midResults.push_back(result);
    }
    
    //add mid-results up
    result = 0;
	/*
    Iterate(it, midResults){
        result += *it;
    }*/
    


    dummyInt = result;
    return dummyInt;
}
BigUnsignedInteger BigUnsignedInteger::operator *=(const BigUnsignedInteger& number){
#ifndef OPT
    vector<BigUnsignedInteger> midResults;
    BigUnsignedInteger result;
    BigUnsignedInteger dummyInt(*this);
    vui val = dummyInt.getValue();      //temporary container to work on
    cout << "nonoptimized" << endl;
    //ready the operands, make #digits equal
    if(number.getValue().size() != val.size()){   //if #digits not equal
        if(val.size() < number.getValue().size()){  //if rhs has more digits
            int difference = number.getValue().size() - val.size();
            for(int i=0 ; i<difference ; i++)
                val.push_back(0);   //make lhs equal to rhs, digit-wise
        }                           
    }
    
    //get the mid-results of multiplication
    for(int i=0; i<val.size() ; i++){
        result = dummyInt*number.getValue()[i];
        for(int j=0 ; j<i ; j++){   //optimization IS required
            result = result*LIMIT + result;
        }
        midResults.push_back(result);
    }
    
    //add mid-results up
    result = 0;
    Iterate(it, midResults){
        result += *it;
    }
    
    *this = result;
#else
    BigUnsignedInteger right;
    BigUnsignedInteger result;
    BigUnsignedInteger left;
    left = *this;
    right = number;
    
    //place smaller (to be right shifted) to the left to increase speed
    if((left <= right) == false){   //if left > right
        result = left;
        left = right;
        right = result;
        result = 0; //reset result
    }
    
    if(left.getValue()[0]%2 == 1)
        result += right;
    while((left <= 1) == false){    //while left > 1
        left = left>>1;
        right = right<<1;
        if(left.getValue()[0] % 2 != 0){
            result += right;
        }
    }
    *this = result;
#endif
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator *(const unsigned int& number){
    BigUnsignedInteger dummyInt;
#ifndef OPT
    if(value.size() == 1 && value[0]==0){   //if first multiplicand is zero
        dummyInt = 0;
        return dummyInt;        // return 0
    }
    else if(value.size() == 1 && value[0]==1){
        dummyInt = number;
        return dummyInt;
    }
    
    //else
    for(int i=0; i<number ; i++){   //do the multiplication
        dummyInt += *this;
    }
#else
    BigUnsignedInteger right;
    BigUnsignedInteger result;
    BigUnsignedInteger left;
    left = *this;
    right = number;
    
    //place smaller (to be divided) to the left to increase speed
    if((left <= right) == false){   //if left > right
        result = left;
        left = right;
        right = result;
        result = 0; //reset result
    }
    
    if(left.getValue()[0]%2 == 1)
        result += right;
    while((left <= 1) == false){    //while left > 1
        left = left>>1;
        right = right<<1;
        if(left.getValue()[0] % 2 != 0){
            result += right;
        }
    }
    dummyInt = result;
#endif
    return dummyInt;
}
BigUnsignedInteger BigUnsignedInteger::operator *=(const unsigned int& number){
    BigUnsignedInteger dummyInt;
#ifndef OPT
    for(int i=0; i<number ; i++){
        dummyInt += *this;
    }
    *this = dummyInt;
#else
    BigUnsignedInteger right;
    BigUnsignedInteger result;
    BigUnsignedInteger left;
    left = *this;
    right = number;
    
    //place smaller (to be divided) to the left to increase speed
    if((left <= right) == false){   //if left > right
        result = left;
        left = right;
        right = result;
        result = 0; //reset result
    }
    
    if(left.getValue()[0]%2 == 1)
        result += right;
    while((left <= 1) == false){    //while left > 1
        left = left>>1;
        right = right<<1;
        if(left.getValue()[0] % 2 != 0){
            result += right;
        }
    }
    *this = result;
#endif
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator-(const unsigned int& number){
    //the number provided is assumed to be less than the *this' value
    //otherwise, the protection mechanism kicks in, prevents unsigned
    //value's circular behaviour by fixing it to 0.
    //this mechanism, however, is not implemented for subtraction
    //of 2 BigUnsignedInteger classes
    
    //dummy variables, dummy and val
    BigUnsignedInteger dummy;
    vui val = value;
    
    //if borrow occurs && there are digits to borrow
    if((signed long int)(val[0])-number < 0 && val.size() != 1){ 
        val[0] = val[0]+(LIMIT-number+1); //set the least significant digit
        if(val.size() > 1){         //if there are more than 1 digit
            int i;
            for(i=1; i<val.size() ; i++){   //iterate through digits
                if(val[i] == 0){    //if next digit is zero
                    val[i] = LIMIT; //set its value
                    continue;       //move to next digit
                }
                else{               //if not zero
                    val[i] = val[i]-1;  //decrement by 1
                    break;          //borrow found, stop iteration
                }
            }
            
            //if the last digit (that is borrowed) is zero
            if(i == val.size()-1 && val[i] == 0)
                val.pop_back();     //remove the last digit
        }
    }
    else if((signed long int)(val[0])-number >= 0) //no borrow
        val[0] -= number;
    
    dummy.setValue(val);
    return dummy;
}
BigUnsignedInteger BigUnsignedInteger::operator-=(const unsigned int& number){
    //the number provided is assumed to be less than the *this' value
    //otherwise, the protection mechanism kicks in, prevents unsigned
    //value's circular behaviour by fixing it to 0.
    //this mechanism, however, is not implemented for subtraction
    //of 2 BigUnsignedInteger classes
    
    //dummy variables, dummy and val
    BigUnsignedInteger dummy;
    vector<unsigned int> val = value;
    
    //if borrow occurs && there are digits to borrow
    if((signed long int)(val[0])-number < 0 && val.size() != 1){ 
        val[0] = val[0]+(LIMIT+1-number); //set the least significant digit
        if(val.size() > 1){         //if there are more than 1 digit
            int i;
            for(i=1; i<val.size() ; i++){   //iterate through digits
                if(val[i] == 0){    //if next digit is zero
                    val[i] = LIMIT; //set its value
                    continue;       //move to next digit
                }
                else{               //if not zero
                    val[i] = val[i]-1;  //decrement by 1
                    break;          //borrow found, stop iteration
                }
            }
            
            //if the last digit (that is borrowed) is zero
            if(i == val.size()-1 && val[i] == 0)
                val.pop_back();     //remove the last digit
        }
    }
    //else if no borrow
    else if((signed long int)(val[0])-number >= 0){ 
        val[0] -= number;
    }
    
    
    dummy.setValue(val);
    *this = dummy;
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator-(const BigUnsignedInteger& number){

    BigUnsignedInteger dummy;
    if(*this == number) return dummy;
    if(number.length() > this->length()){   //negative value, therefore return 0
        return dummy;
    }
    vui val = value;
    int i;
    for(i=0 ; i<number.getValue().size() ; i++){    //for each digit
        if((signed long int)(val[i])-number.getValue()[i] < 0){ //borrow occurs  
            //set the digit
            val[i] = val[i]+((LIMIT - number.getValue()[i]) + 1); 
            
            if(val.size() > i+1){   //if there are more than 1 digit
                int j;
                for(j=i+1; j<val.size() ; j++){   //iterate through digits
                    if(val[j] == 0){    //if next digit is zero
                        val[j] = LIMIT; //set its value
                        continue;       //move to next digit
                    }
                    else{               //if not zero
                        val[j] = val[j]-1;  //decrement by 1
                        break;          //borrow found, stop iteration
                    }
                }

                //if the last digit (that is borrowed) is zero
                if(j == val.size()-1 && val[j] == 0)
                    val.pop_back();     //remove the last digit
            }
        }
        else{
            val[i] -= number.getValue()[i];
            if(val[i] == 0){    //if last digit is 0, remove it
//                cout << "val[" << i << "]=0" << endl;
//                cout << "****" << endl << "valsize=" << val.size() << endl;
                if(i==val.size()-1){
                    val.pop_back();
                }
            }
        }
    }
    dummy.setValue(val);
    return dummy; 
}
BigUnsignedInteger BigUnsignedInteger::operator-=(const BigUnsignedInteger& number){
    
    BigUnsignedInteger dummy;
    if(*this == number){ *this = dummy; return *this;}
    if(number.length() > this->length()){   //negative value, therefore return 0
        return dummy;
    }
    vui val = value;
    unsigned int i;
    for(i=0 ; i<number.getValue().size() ; i++){    //for each digit
        if((signed long int)(val[i])-number.getValue()[i] < 0){ //borrow occurs            
            //set the digit
            val[i] = val[i]+(LIMIT+1-number.getValue()[i]); 
            
            if(val.size() > i+1){   //if there are more than 1 digit
                int j;
                for(j=i+1; j<val.size() ; j++){   //iterate through digits
                    if(val[j] == 0){    //if next digit is zero
                        val[j] = LIMIT; //set its value
                        continue;       //move to next digit
                    }
                    else{               //if not zero
                        val[j] = val[j]-1;  //decrement by 1
                        break;          //borrow found, stop iteration
                    }
                }

                //if the last digit (that is borrowed) is zero
                if(j == val.size()-1 && val[j] == 0)
                    val.pop_back();     //remove the last digit
            }
        }
        else{
            val[i] -= number.getValue()[i];
            if(val[i] == 0){
//                cout << "val[" << i << "]=0" << endl;
//                cout << "****" << endl << "valsize=" << val.size() << endl;
                if(i==val.size()-1){
                    val.pop_back();
                }
            }
        }
    }
    dummy.setValue(val);
    *this = dummy;
    return *this; 
}
BigUnsignedInteger BigUnsignedInteger::operator/(const unsigned int& number){
    //dummy variables, dummy and val
    BigUnsignedInteger dummy(*this);
    BigUnsignedInteger division;
    
    if(number == 1)
        return *this;
    
    while(dummy.length() > 32){    //subtract until it has unsigned int's size
        dummy -= number;
        division += 1;
    }
    
    while(dummy.val() > 0){
        if(dummy.val() < number)
            break;
        else{
            dummy -= number;
            division += 1;
        }
    }
    return division;
}
BigUnsignedInteger BigUnsignedInteger::operator/=(const unsigned int& number){
    //dummy variables, dummy and val
    BigUnsignedInteger dummy(*this);
    BigUnsignedInteger division;
    
    if(number == 1)
        return *this;
    
    while(dummy.length() > 32){    //subtract until it has unsigned int's size
        dummy -= number;
        division += 1;
    }
    
    while(dummy.val() > 0){
        if(dummy.val() < number)
            break;
        else{
            dummy -= number;
            division += 1;
        }
    }
    *this = division;
    return *this;
}
BigUnsignedInteger BigUnsignedInteger::operator/(const BigUnsignedInteger& number){
    BigUnsignedInteger division;
    if(number.length() == 1 && number.val() == 1){
        division = *this;
        return division;
    }
#ifndef DIV
    BigUnsignedInteger dummy(*this);
    
    if(number.length() == 1){
        if(number.val() == 1)
            return *this;
    }
    
    while(dummy.length() > 32){  //subtract until it has unsigned int's size
        dummy -= number;
        division += 1;
    }
    
    while(dummy.val() > 0){ //dummy will either be 0 or (0 < dummy < number)
        if(dummy.val() < number.val())
            break;
        else{
            dummy -= number;
            division += 1;
        }
    }
#else
//    cout << "dividing "; print();
//    cout << "with     "; number.print();
    BigUnsignedInteger denom = number;
    BigUnsignedInteger nom = *this;
    BigUnsignedInteger result;
    BigUnsignedInteger dummy;
    dummy = 1;
    unsigned int shiftCount = 0;
    unsigned int coef = 0;
    
    while(denom <= nom){
        shiftCount++;
        denom = denom << 1;
    }
    shiftCount--;
    denom = denom >> 1;
//    cout << "largest val < nom "; denom.print();
//    cout << "shifCount = " << shiftCount << endl;
    
    while((nom <= 1)==false){   //nom > 1
//        cout << "noma= "; nom.print();
//        cout << "denoma= "; denom.print();
        while((nom < denom) == false){ //while nom >= denom
            nom -= denom;
            coef++;
//            cout << "while nom="; nom.print();
//            char ch;
//            cin >> ch;
        }
//        cout << "difference = "; nom.print();
//        cout << "coefficient = " << coef << endl;
        for(int i=0; i<shiftCount ; i++) dummy = dummy << 1;
//        cout << "dummy = "; dummy.print();
        result += dummy*coef;
//        cout << "result = "; result.print();
        shiftCount--;
        denom = denom >> 1;
        coef = 0;
        dummy = 1;
//        cout << endl;
    }
    division = result;
#endif
    return division;
}
BigUnsignedInteger BigUnsignedInteger::operator/=(const BigUnsignedInteger& number){

#ifndef DIV
    BigUnsignedInteger dummy(*this);
    BigUnsignedInteger division;
    if(number.length() == 1){
        if(number.val() == 1)
            return *this;
    }
    
    while(dummy.length() > 32){  //subtract until it has unsigned int's size
        dummy -= number;
        division += 1;
    }
    
    while(dummy.val() > 0){ //dummy will either be 0 or (0 < dummy < number)
        if(dummy.val() < number.val())
            break;
        else{
            dummy -= number;
            division += 1;
        }
    }
    *this = division;
#else
    /************************************************************************
     * Optimized division algorithm is based on long division in binary.    *
     * The algorithm works this way: Denominator is left shifted until      *
     * it is bigger than the nominator while the shift count is tracked.    *
     * after that denominator shifted right once for getting a smaller      *
     * value than the denominator. After that, the regular repetitive       *
     * subtraction method is used for the division. At each subtraction     *
     * coefficient is incremented (starting from 1). After the sub-division *
     * is complete, shift count * coefficient is added to result.           *
     * This goes on until the nominator is 1 or 0. Illustration is below:   *
     *                                                                      *
     *  21 / 2                                                              *
     * 2-> 4 -> 8 -> 16 -> 32 (>21) -> 16. shift count = 3                  *
     * 21 - 16 = 15 (<16). coefficient = 1. Result += 1(coef) * 2^3(dummy)  *
     * denominator -> 8; coefficient = 0; nominator = 15; shift count = 2   *
     * 15 - 8 = 7 (<8). coefficient=1, result += 1*2^2;                     *
     * goes on and on until nominator is 1 or 0. Therefore the division     *
     * will have a complexity of O(logn) instead of O(n)                    *
     ***********************************************************************/  
    
//    cout << "dividing "; print();
//    cout << "with     "; number.print();
    BigUnsignedInteger denom = number;
    BigUnsignedInteger nom = *this;
    BigUnsignedInteger result;
    BigUnsignedInteger dummy;   //for mid results
    dummy = 1;
    unsigned int shiftCount = 0;
    unsigned int coef = 0;
    
    while(denom < nom){
        shiftCount++;
        denom = denom << 1;
    }
    shiftCount--;
    denom = denom >> 1;
//    cout << "largest val < nom "; denom.print();
//    cout << "shifCount = " << shiftCount << endl;
    
    while(denom < nom && (nom <= 0)==false){   //nom > 0
//        cout << "nom= "; nom.print();
//        cout << "denom= "; denom.print();
        while((nom < denom) == false){ //while nom >= denom
            nom -= denom;
            coef++;
        }
//        cout << "difference = "; nom.print();
//        cout << "coefficient = " << coef << endl;
        for(int i=0; i<shiftCount ; i++) dummy = dummy << 1;
//        cout << "dummy = "; dummy.print();
        result += dummy*coef;
//        cout << "result = "; result.print();
        shiftCount--;
        denom = denom >> 1;
        coef = 0;
        dummy = 1;
//        cout << endl;
    }
    *this = result;
#endif
    return *this;
}
bool BigUnsignedInteger::operator==(const BigUnsignedInteger& number){
    if(value.size() != number.getValue().size()){
        return false;   //if digits are not equal return false
    }
    else{   //if they have same number of digits
        for(int i=0; i<number.getValue().size() ; i++){ //check each digit
            if(value[i] != number.getValue()[i]){    //if not equal
                return false;
            }
        }   //if all are equal
        return true;
    }
}
bool BigUnsignedInteger::operator==(const unsigned int& number){
    if(value.size() > 1)    //if more than 1 digit
        return false;
    else{   //if big unsigned int has 1 digit
        if(value[0] != number)  //compare values
            return false;
        else
            return true;
    }
}
bool BigUnsignedInteger::operator<(const BigUnsignedInteger& number){
    if(*this == number) return false;
    if(value.size() < number.getValue().size()) return true;
    else if(value.size() > number.getValue().size()) return false;
    else{   //if equal size
        for(int i=value.size()-1 ; i>=0 ; i--){ //start from most sig. digit
            if(value[i] < number.getValue()[i]) return true;
            else if(value[i] > number.getValue()[i]) return false;
            else continue;
        }
    }
}
bool BigUnsignedInteger::operator<(const unsigned int& number){
    if(value.size() > 1) return false;
    else return value[0] < number;
}
bool BigUnsignedInteger::operator<=(const BigUnsignedInteger& number){
    if(*this < number || *this == number) return true;
    else return false;
}
bool BigUnsignedInteger::operator<=(const unsigned int& number){
    if(*this < number || *this == number) return true;
    else return false;
}
BigUnsignedInteger BigUnsignedInteger::operator>>(const unsigned int number){
    BigUnsignedInteger dummy(*this);
    vui val = value;
    unsigned int size = value.size();
    unsigned int a;
    unsigned int remainder=0;     //value to be transferred to lower digits
    for(int j=0; j<number; j++){        //shift number of times
        for(int i=size-1; i>=0 ; i--){  //digit loop
            a = val[i];
            val[i] = val[i] >> 1;
            val[i] += remainder;
            if(a%2 == 1) remainder = 2147483648; // 0x80000000
            else    remainder = 0;
            a = a >> 1;
            if(i == size-1 && a == 0){
                val.pop_back();
            }
        }
        size = val.size();
        remainder = 0;
    }
    dummy.setValue(val);
    return dummy;
}
BigUnsignedInteger BigUnsignedInteger::operator<<(const unsigned int number){
    BigUnsignedInteger dummy(*this);
    vui val = value;
    unsigned int size = value.size();
    unsigned long long a;       //carry test variable
    unsigned int carry = 0;
    for(int j=0 ; j<number ; j++){  //shift number of times
        for(int i=0; i<size; i++){  //original size is used since adding
            a = val[i];             //digits will increase val.size()
            val[i] = val[i] << 1;   //left shift
            val[i] += carry;        //add carry
            if(a << 1 > LIMIT){ //if overflow occurs, set carry
                carry = 1;
                if(i == size-1) val.push_back(1); //increase digit# when necess.
            }
            else
                carry = 0;  //no carry
        }
        size = val.size();  //set new size, since digits may be increased.
        carry = 0;
    }
    
    dummy.setValue(val);
    return dummy;
}