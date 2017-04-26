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
    chord.InitChord(5);
	chord.AddPeer(3);
    chord.AddPeer(5);
	chord.AddPeer(7);
	chord.AddPeer(8);
	chord.AddPeer(30);
    return 0;
}
