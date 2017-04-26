//
//  Chord.hpp
//  Chord
//
//  Created by Asjad Athick on 26/4/17.
//  Student: 4970512
//  Compilation: g++
//  Copyright © 2017 asjad.io. All rights reserved.
//

#ifndef Chord_hpp
#define Chord_hpp

#include <iostream>
#include "Peer.hpp"

const unsigned short MAX_CHORD_N_SIZE = 32;

class Chord{
private:
    unsigned int chordSize;
    Peer *index;
public:
    Chord();
    void InitChord(unsigned int n);
};

#endif /* Chord_hpp */
