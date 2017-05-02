//
//  main.cpp
//  Chord
//
//  Created by Asjad Athick on 26/4/17.
//  Student: 4970512
//  Compilation: g++
//  Copyright © 2017 asjad.io. All rights reserved.
//

#include "Chord.hpp"

using namespace std;
int main(int argc, const char * argv[]) {

	try {
		if (argc < 2) {
			throw std::string("Input file not specified in arguments");
		} else {
			Chord chord;
			chord.Read(argv[1]);
		}
	} catch (std::string ex) {
		cout << "Program ended with exception: " << ex << std::endl;
	}

    return 0;
}
