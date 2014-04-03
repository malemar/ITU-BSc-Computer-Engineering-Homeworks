#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>


using namespace std;

// function prototypes			  
void makeFile();
bool isDouble(const char*);
bool isUnsigned(const char*);
bool readFile(const string&, vector<string>&);
void print(const vector<string>&);		   
void print(const vector<string>&, const vector<string>&);
void encode(const vector<string>&, vector<string>&);
unsigned countOnes(const string&, const unsigned, const unsigned);
void send(const vector<string>&, vector<string>&, const double, const unsigned);
unsigned decode(const vector<string>&, const vector<string>&, vector<string>&, unsigned*);
unsigned countErrors(const vector<string>&, const vector<string>&);

int main(int argc, char** argv){
	///
	/// PARAMETER CHECK
	///
	if(argc < 4){
		cout << "Invalid input sequence. Try \"./a.out \"inputName\" errorRate burstSize\"" << endl;
		getchar();
		return -1;
	}
	 
	//check numerical parameters
	double errorRate;
	unsigned burstSize;
	if(isDouble(argv[2])){
		errorRate = atof(argv[2]);
		if(errorRate > 1){
			cout << "Error rate cannot be larger than 1" << endl;
			getchar();
			return -1;
		}
	}
	else{
		cout << "Argument \"" << argv[2] << "\" contains invalid characters. It should be a double." << endl;
		getchar();
		return -1;
	}

	if(isUnsigned(argv[3]))
		burstSize = atoi(argv[3]);
	else{
		cout << "Argument \"" << argv[3] << "\" contains invalid characters. It should be an unsigned integer." << endl;
		getchar();
		return -1;
	}

	cout << "--------------------------------------------------" << endl;
	cout << "Error Rate: " << errorRate << endl;
	cout << "Burst Size: " << burstSize << endl;						  
	cout << "--------------------------------------------------" << endl << endl;

	srand((unsigned)time(NULL));
	///
	/// INITIALIZE PROGRAM DATA
	///
	vector<string> sender;			//  8 bits, read from file
	vector<string> codedFrames;		// 12 bits, coded frames/codeword
	vector<string> receiver;		// 12 bits, received erronous frames
	vector<string> correctedFrames;	// 12 bits, corrected from the received ones
	unsigned detectedErrors;
	unsigned correctedErrors;
	unsigned allErrors;
	unsigned miscorrectedErrors = 0;

	// Read the file
	if(readFile(argv[1], sender) == false){
		cout << "Error reading file: " << argv[1] << endl;
		getchar();
		return -1;
	}

	sender[0] = "10100010";		// test input
	encode(sender, codedFrames);
	send(codedFrames, receiver, errorRate, burstSize);
	correctedErrors = decode(codedFrames, receiver, correctedFrames, &miscorrectedErrors);
	detectedErrors = correctedErrors;	// since we use SEC
	allErrors = countErrors(codedFrames, receiver);

	cout << "--------------------------------------------------" << endl;
	cout << "Total error count:\t" << allErrors << endl;
	cout << "Detected error count:\t" << detectedErrors << endl;
	cout << "Corrected error count:\t" << correctedErrors << endl;
	cout << "Miscorrection count:\t" << miscorrectedErrors << endl;
	cout << "--------------------------------------------------" << endl;
	cout << "Error detection ratio:\t\t" << ((double)detectedErrors)/allErrors << endl;
	cout << "Error correction ratio:\t\t" << ((double)correctedErrors)/allErrors << endl;
	cout << "Error miscorrection ratio:\t" << ((double)miscorrectedErrors)/allErrors << endl;
	cout << "--------------------------------------------------" << endl << endl; 

	getchar(); 
	return 0;
}

 void makeFile(){	// create random input file
	FILE* fp = fopen("input.txt", "w");
	
	srand( (unsigned)time(NULL));
	for(int i=0; i<100; ++i){	  // rows
		for(int j=0; j<8 ; ++j)	  // bits
			fputc(rand()%2+48, fp);
		fputc('\n', fp);
	}

	fclose(fp);
	return;
}

 bool isDouble(const char* s){
	 int i=0;
	 while(s[i] != '\0'){
		if(!isdigit(s[i]) && s[i] != '.' && s[i] != ',')
			return false;
		++i;
	 }
	 return true;
 }

 bool isUnsigned(const char* s){
    int i=0;
	while(s[i] != '\0'){
	if(!isdigit(s[i]))
		return false;
	++i;
	}
	return true;
 }

bool readFile(const string& s, vector<string>& sender){
	ifstream input(s.c_str());
	if(input.is_open()){
		for(string line; getline(input, line); )
			sender.push_back(line);
	}
	else
		return false;
	input.close();
	return true;
}

void print(const vector<string>& vec){
	cout << "Printing Vector:" << endl;
	for(vector<string>::const_iterator it = vec.begin(); it != vec.end() ; ++it)
		cout << *it << endl;
	cout << endl;
	return;
}

void print(const vector<string>& v1, const vector<string>& v2){
	cout << "Printing Vectors: " << endl;
	for(int i=0; i<100 ; i++)
		cout << v1[i] << "\t" << v2[i] << endl;
	cout << endl;
	return;
}

unsigned countOnes(const string& s, const unsigned start, const unsigned limit){
	unsigned count = 0;

	for(unsigned i=start ; i<limit && i<s.size(); i++){
		if(s[i] == '1')
			count++;
	}
	return count;
}

void encode(const vector<string>& frame, vector<string>& codedFrame){
	// place the data bits first, skipping parity bits
	codedFrame.resize(100);			// 100 lines
	for(int k=0 ; k<100 ; ++k){
		codedFrame[k].resize(12);	// each line 12 bits
		int j=0;
		for(int i=0; i<12 ; ++i){
			if(i==0 || i==1 || i==3 || i==7){   // skip parity bits
				codedFrame[k][i] = '-';
				continue;
			
			}
			codedFrame[k][i] = frame[k][j];
			++j;
		}
	}

	// calculate parity bits
	for(int k=0 ; k<100 ; ++k){			// for every line
		for(int i=8; i>0 ; i >>= 1){	// for every parity bit
			unsigned count = 0;
			unsigned limit = i*2-1;
			unsigned start = i-1;
			while(start<12){
				count += countOnes(codedFrame[k], start, limit);
				start += i*2;
				limit += i*2;
			}
			count%2 == 0 ? codedFrame[k][i-1] = '0' : codedFrame[k][i-1] = '1';
		}
	}
	return;
}

void send(const vector<string>& sender, vector<string>& receiver, const double errorRate, const unsigned burstSize){
	// first send the coded frames, apply the errors later
	receiver = sender;

	// single bit errors
	if(burstSize == 1){
		cout << "Single-bit error applied." << endl;
		for(unsigned k=0; k<receiver.size(); k++){			// for every frame
			for(unsigned j=0; j<receiver[k].size(); j++){	  // for all 12 bits
				if(((double)(rand()%100)) / 100 < errorRate)	// invert the bit if rand() lets
					receiver[k][j] == '1' ? receiver[k][j] = '0' : receiver[k][j] = '1';
			}
		}
	}
	else{	// burst errors
		cout << "Burst error applied." << endl;
		for(unsigned k=0; k<receiver.size() ; k++){		// for every frame
			if(((double)(rand()%100))/100 < errorRate){	// if rand() lets burst to occur
				unsigned index = rand() % (receiver[k].size()-burstSize);  // starting index of burst errors
				//cout << "index = " << index << "\tburst = " << burstSize << endl;
				//cout << receiver[k] << "\t";
				// invert first and last bit of the burst 
				// first bit
				receiver[k][index] == '1' ? receiver[k][index] = '0' : receiver[k][index] = '1';	
				receiver[k][index+burstSize-1]=='1' ? 		 // last bit of the burst
					receiver[k][index+burstSize-1]='0' : 
					receiver[k][index+burstSize-1]='1';

				// apply randomness the the remaining burst bits (index, limit)
				unsigned count = 2;
				for(unsigned j = index+1 ; j < index + burstSize-1 ; ++j){ // for the burst interval
					if(((double)(rand()%100))/100 < errorRate){	// if error rate lets, invert the remaining burst bits
						receiver[k][j] == '1' ? receiver[k][j] = '0' : receiver[k][j] = '1'; 
						++count;
					}
				}
				//cout << receiver[k] << "\tcount = " << count << endl << endl;
			}
		}
	}

	return;
}

unsigned decode(const vector<string>& codedFrames, const vector<string>& received, vector<string>& corrected, unsigned* miscorrected){
	unsigned correctedCount = 0;
	corrected = received;
	
	for(unsigned k=0; k<corrected.size(); ++k){	  // for every frame
		// check parity bits
		unsigned parityCheck = 0;
		for(unsigned j=1 ; j<=8 ; j <<= 1){	 //1,2,4,8
			unsigned oneCount = 0;
			unsigned limit = j*2-1;
			unsigned start = j-1;
			while(start<12){
				oneCount += countOnes(corrected[k], start, limit);
				start += j*2;
				limit += j*2;
			}
			if(oneCount%2 != 0)	// if syndrome, add to parityCheck
				parityCheck += j;
		}
		if(parityCheck != 0){   // correct the position syndrome points to
			if(parityCheck > 12){	// errors made codeword illegal
				//cout << codedFrames[k] << "\t" << corrected[k] << endl;
				cout << "Illegal codeword generated due to errors (syndrome index > 12)" << endl;
			}
			else{
				corrected[k][parityCheck-1] == '1' ? corrected[k][parityCheck-1] = '0' : corrected[k][parityCheck-1] = '1';
				++correctedCount;
				if(corrected[k] != codedFrames[k])	  //compare corrected with original
					*miscorrected += 8;			// if not the same, miscorrection occured
			}
		}
	}

	return correctedCount;
}		

unsigned countErrors(const vector<string>& s, const vector<string>& r){
	unsigned count = 0;
	for(unsigned i=0; i<s.size() ; ++i){		// compare each frame
		for(unsigned j=0; j<s[i].size() ; j++){ // compare each bit
			if(s[i][j] != r[i][j])
				++count;
		}
	}
	return count;
}
