#include "ALU.h"
#include <iostream>
using namespace std;
Value<3> toValue(ALUop op) {
    return {static_cast<unsigned long>(op)};
}

ALU::ALU() {
    // implement ALU constructor here
	inputs[0] = 0;
	inputs[1] = 0;
	output = 0;

	zflag = 0;
	nflag = 0;
	oflag = 0;
	cflag = 0;
}

Value<1> ALU::get_output(int output, Value<1> ignore) const {
    // return condition code associated with output
/*  	switch (output){
		case 0:
			return zflag;
		case 1:
			return nflag;
		case 2:
			return oflag;
		case 3:
			return cflag;
		default:
 */
			return 0;
//	}
}

word ALU::get_output() const {
    // return computed value from ALU
	return output;
}

void ALU::update_input(int num, Value<3> value) {
    // receive new operation
	cout << "changing operand to: " << value << endl;
	operand = value;
	operate(operand);
	cout << "Calling update_listeners()\n";
	Reporter::update_listeners();
	cout << "Done calling update_listeners()\n";
	MultiReporter::update_listeners();
}

void ALU::update_input(int num, word value) {
    // receive new input value
	cout << "num input = :" << num << " : changing value to: " << value << endl;
	inputs[num] = value;
	operate(operand);
	cout << "Calling update_listeners()\n";
	Reporter::update_listeners();
	cout << "Done calling update_listeners()\n";
	MultiReporter::update_listeners();
}

void ALU::operate(int op){
	switch (op){
		case 0:
			output = inputs[0] << inputs[1];
			break;
		case 1:
			output = ~(inputs[0] | inputs[1]);
			break;
		case 2:
			output = (inputs[0] | inputs[1]);
			break;
		case 3:
			output = inputs[0] >> inputs[1];
			break;
		case 4:
			if (inputs[0] < inputs[1]){
				output = 1;
				cout << "if input 1 is less then input 2 :" << inputs[0] << ", " << inputs[1] << endl;
			}
			else{
				output = 0;
				cout << "if input 1 is greater then input 2 :" << inputs[0] << ", " << inputs[1] << endl;
			}
			break;
		case 5:
			output = inputs[0] - inputs[1];
			break;
		case 6:
			output = inputs[0] + inputs[1];		
			break;
		case 7:
			output = (inputs[0] ^ inputs[1]);		
			break;
	}
	if (output == 0){
		zflag = 1;
	}
	/*
	if (output < 0){
		nflag = 1;
	}
	
	int a1 = inputs[0];
	int b1 = inputs[1];
	int co = 0;
	int a = a1 & 1;
	int b = b1 & 1;
	for(int i=0; i < 31; i++){
		co = ((a ^ b) & co) | (a & b);
		a1 = a1 >> 1;
		b1 = b1 >> 1;
		a = a1 & 1;
		b = b1 & 1;
	}
	co = ((a ^ b) & co) | (a & b);
	if (co == 1) {
		cflag = 1;
	}
	else {
	cflag = 0;
	}
	if ((a >= 0 and b < 0) or (b >=0 and a < 0)){
		oflag = 0;
	}
	else if ((inputs[0] >= 0 and inputs[1] >= 0) or (inputs[0] < 0 and inputs[1] < 0)){
		if (inputs[0] >= 0){
			if (a == 1) {
				oflag = 1;
			}
			else{
				oflag = 0;
			}
		}
		else if (inputs[0] < 0){
			if (a == 0) {
				oflag = 1;
			}
			else{
				oflag = 0;
			}	
		}
	}
	*/
}
