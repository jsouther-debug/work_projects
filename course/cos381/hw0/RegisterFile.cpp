#include "RegisterFile.h"

RegisterFile::RegisterFile(Clock* clock) {
	for(int i = 0; i < 32; i++){
		rfile[i] = Register(clock);
	}
}

void RegisterFile::update_input(int num, word value) {
	if (num != 0 and rfile[num].enable = 1){
		rfile[num].next_value = value;
	}
}

void RegisterFile::update_input(int num, Value<5> value) {
//	rfile[num]
}

void RegisterFile::update_input(int num, Value<1> value) {

}

word RegisterFile::get_output(int output, word ignore) const {
}
