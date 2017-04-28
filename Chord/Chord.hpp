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

#include <fstream>
#include <algorithm>
#include "Peer.hpp"

const unsigned short MAX_CHORD_N_SIZE = 32;

unsigned int hash(std::string input, unsigned int chordSize);

class Chord{
private:
    unsigned int chordSize;
    Peer *index;
	void ChangeData(std::string value, bool insert=true);
	void checkInit();
	void checkKeyRange(unsigned int key);

	//reader functions
	void  trimCommentsAndTrailingSpace(std::string&);
public:
    Chord();
    void InitChord(unsigned int n);
    void AddPeer(unsigned int id);
    void RemovePeer(unsigned int id);
    void FindKey(unsigned int key, Peer *&foundPeer);
	void Insert(std::string value);
	void Delete(std::string value);
	void Print(unsigned int key);
	void UpdateAllFingerTables();
	void Read(std::string filename);
	~Chord();
};

#endif /* Chord_hpp */
