//
//  Chord.cpp
//  Chord
//
//  Created by Asjad Athick on 26/4/17.
//  Student: 4970512
//  Compilation: g++
//  Copyright © 2017 asjad.io. All rights reserved.
//

#include "Chord.hpp"

Chord::Chord(){
    chordSize = 1;
    index = NULL;
}

void Chord::InitChord(unsigned int n){
    
    //size check
    if (n < 1 || n > MAX_CHORD_N_SIZE) {
        throw std::string ("InitChord: Cannot initialise Chord; invalid size");
    }
    
    this->chordSize = n;
    
    //print details
    std::cout << "Asjad Athick" << std::endl;
    std::cout << "4970512" << std::endl;
    
    //check if re initialised
    if (index != NULL) {
        delete [] index;
    }
    
    //new node at 0
    index = new Peer(0, this->chordSize);
    index->setSuccessor(index);
}


