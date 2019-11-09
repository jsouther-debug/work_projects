#include "RegisterFile.h"

RegisterFile::RegisterFile(Clock* clock) {
	for(int i = 0; i < 32; i++){
		rfile.push_back(Register<32>(clock));
		rfile[i].register_listener(this, i+5);
	}
}

void RegisterFile::update_input(int num, word value) {
	if (num == 3){
		if (rd != 0){
			rfile[rd].update_input(0, value);
			rfile[rd].update_input(0, Value<1>{1});
		}
		next_val = value;
	}
	else{
		update_listeners();
	}
}

void RegisterFile::update_input(int num, Value<5> value) {
	switch(num){
		case 0:
			r1 = value;
			break;
		case 1:
			r2 = value;
			break;
		case 2:
			rfile[rd].update_input(40 Value<1>{0});
			rd = value;
			if (rd != 0){
				rfile[rd].update_input(0, Value<1>{1});
			}
			rfile[rd].update_input(0, next_val);
			break;
	}
}

void RegisterFile::update_input(int num, Value<1> value) {
	if (num == 4){
		we = value;
		if (rd != Value<5>{0}){
			rfile[rd].update_input(0, we);
		}
	}
}

word RegisterFile::get_output(int output, word ignore) const {
	switch (output){
		case 0:
			return rfile[r1].get_output();
		case 1:
			return rfile[r2].get_output();
		default:
			return 0;
	}
}
