/****************************************/
/*	BLG381E - Advanced Data Structures	*/
/*				Fall 2012				*/
/*										*/
/*			Volkan Ilbeyli				*/
/*			   040100118				*/
/*										*/
/*		Insturctor: Zehra Cataltepe		*/
/****************************************/

//NOTE: linear hashing and double hashing of files result in different collision
//		counts on windows and linux. The plot files provided are generated using
//		windows collision counts.

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

int linear_hash(int*, int, int);
int double_hash(int*, int, int);
int find_prime(int);
bool isPrime(int);
void print(int*, int);
					//results are in the report for given key_set[];
					//LINEAR
				   //1, 2 , 5, 3 , 6 , 9, 10, 7 , 11, 12, 4
					//DOUBLE
				   //1, 0...
int main(){		   //1, 1 , 5, 3 , 5 , 9, 9 , 7 , 10, 12, 1
	int key_set[] = {1, 27, 5, 42, 18, 9, 35, 72, 49, 12, 92};
	int table[13] = {0};	//assuming given keyset doesnt contain 0 as key value;
	char c;

	ifstream source;
	ofstream target;
	string line;
	int key;
	int table2[1327] = {0};
	int file1_key_set[1000], file2_key_set[1000]; 
	int file1_collision_count[1000] = {0};
	int file2_collision_count[1000] = {0};
	int i;

	//read files
	cout << "Reading files. . ." << endl;
	source.open("x1.txt");
	if(!source.is_open())
		cout << "source cannot be opened" << endl;
	else{
		i = 0;
		while(source.good() && i != 1000){
			getline(source,line);
			key = atoi(line.c_str());
			file1_key_set[i] = key;
			i++;
		}
	}
	source.close();

	source.open("x2.txt");
	if(!source.is_open())
		cout << "source cannot be opened" << endl;
	else{
		i = 0;
		while(source.good() && i != 1000){
			getline(source,line);
			key = atoi(line.c_str());
			file2_key_set[i] = key;
			i++;
		}
	}
	source.close();		//reading is done
	cout << "Reading files: DONE!" << endl << endl;

	//linear hash 1st file and write the count of collisions of each element
	cout << "Linear hashing \"x1.txt\" . . ." << endl;
	target.open("linear_count_1.txt");
	for(int j = 0 ; j < 1000 ; j++){
		file1_collision_count[j] = linear_hash(table2, file1_key_set[j], 1327);
		target << file1_collision_count[j];
		target << endl;
	}
	cout << "Linear hashing: DONE!" << endl << endl;
	target.close();
	//double hash 1st file and write the count of collisions of each element
	//reset hash table
	for(int k = 0 ; k < 1327 ; k++)
		table2[k] = 0;
	cout << "Double hashing \"x1.txt\" . . ." << endl;
	target.open("double_count_1.txt");
	for(int j = 0 ; j < 1000 ; j++){
		file1_collision_count[j] = double_hash(table2, file1_key_set[j], 1327);
		target << file1_collision_count[j];
		target << endl;
	}
	cout << "Double hashing: DONE!" << endl << endl;
	target.close();
	
	//linear hash 2nd file and write the count of collisions of each element
	//reset hash table
	for(int k = 0 ; k < 1327 ; k++)
		table2[k] = 0;
	cout << "Linear hashing \"x2.txt\" . . ." << endl;
	target.open("linear_count_2.txt");
	for(int j = 0 ; j < 1000 ; j++){
		file2_collision_count[j] = linear_hash(table2, file2_key_set[j], 1327);
		target << file2_collision_count[j];
		target << endl;
	}
	cout << "Linear hashing: DONE!" << endl << endl;
	target.close();
	//double hash 2nd file and write the count of collisions of each element
	//reset hash table
	for(int k = 0 ; k < 1327 ; k++)
		table2[k] = 0;
	cout << "Double hashing \"x2.txt\" . . ." << endl;
	target.open("double_count_2.txt");
	for(int j = 0 ; j < 1000 ; j++){
		file2_collision_count[j] = double_hash(table2, file2_key_set[j], 1327);
		target << file2_collision_count[j];
		target << endl;
	}
	cout << "Double hashing: DONE!" << endl << endl;
	target.close();

	/*//	TESTING THE GIVEN KEY SET 
	for(i=0 ; i < sizeof(key_set)/4 ; i++)
		cout << double_hash(table, key_set[i], sizeof(table)/4) << endl;

	cout << "print hash table? ";
	cin >> c;
	switch(c){
	case 'y':
	case 'Y':
		print(table, sizeof(table)/4);
		break;
	default:
		break;
	}*/

	fflush(stdin);
	scanf(&c,"%\n");
	return 0;
}

int linear_hash(int* table, int key, int size){	//assuming given keyset doesnt contain 0 as key value;
	int incremental_index = 0;
	int hash_index;
	int count = 0;	//collisoin count

	do{
		hash_index = (key + incremental_index)%size;
		if(table[hash_index] == 0){	//no collision
			table[hash_index] = key;
			break;
		}
		else{	//collision
			count ++;
			//cout << "collision!!" << endl;
			//cout << "key value: " << key << endl << "index: " << hash_index << endl << endl;
			incremental_index++;
		}
	}while(hash_index < size);

	return count;
}

int double_hash(int* table, int key, int size){
	int hash_index1 = key;
	int prime = find_prime(size);
	int hash_index2 = prime - key%prime;
	int incremental_index = 0;
	int hash_index; //hash_index = (hash_index1 + incremental_index*hash_index2)%size;
	int count = 0;  //collision count;
	do{
		hash_index = (hash_index1 + incremental_index*hash_index2)%size;
		if(table[hash_index] == 0){	//no collision
			table[hash_index] = key;
			break;
		}
		else{	//collision
			count++;
			//cout << "collision!!" << endl;
			//cout << "key value: " << key << endl << "index: " << (hash_index1 + incremental_index*hash_index2)%size << endl;
			//cout << "h1: " << hash_index1 << "\t" << "h2: " << hash_index2 << endl << endl;
			incremental_index++;
		}
	}while(hash_index < size);

	return count;
}

int find_prime(int key){
	int i;
	for(i = key; i > 0 ; i--){
		if(isPrime(i) == true)
			break;
	}
	return i;
}

//taken from http://www.cplusplus.com/forum/general/1125/
bool isPrime(int num){	
	if (num <=1)
        return false;
    else if (num == 2)         
        return true;
    else if (num % 2 == 0)
        return false;
    else
    {
        bool prime = true;
        int divisor = 3;
        double num_d = static_cast<double>(num);
        int upperLimit = static_cast<int>(sqrt(num_d) +1);
        
        while (divisor <= upperLimit)
        {
            if (num % divisor == 0)
                prime = false;
            divisor +=2;
        }
        return prime;
    }
}

void print(int* table, int size){
	for(int i = 0 ; i < size ; i++)
		cout << "table[" << i << "] = " << table[i] << endl;

	return;
}
