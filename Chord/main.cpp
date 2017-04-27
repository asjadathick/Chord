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
    Chord chord;
//    chord.InitChord(5);

	//ins
//	chord.AddPeer(7);
//    chord.AddPeer(3);
//	chord.RemovePeer(3);
//	chord.AddPeer(12);
//	chord.AddPeer(3);
//	chord.AddPeer(9);
//	chord.RemovePeer(3);
//	chord.AddPeer(17);
//	chord.Insert("THIS IS A TEST");
//	chord.Delete("THIS IS A TEST");
//
//	chord.FindKey(31);

	//remove
//	chord.RemovePeer(8);
//	chord.RemovePeer(0);
//	chord.RemovePeer(30);
//	chord.RemovePeer(3);

	if (argc < 2) {
		throw std::string("Input file not specified in arguments");
	}

	chord.Read(argv[1]);

    return 0;
}
