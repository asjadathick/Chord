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

unsigned int hash(std::string input, unsigned int chordSize){
    unsigned int key = 0;
    
    for (long i = 0; i < input.size(); i++) {
        key = ((key << 5) + key) ^ input[i];
    }
    
    return key % (int)pow(2, chordSize);
}

Chord::Chord(){
    chordSize = 1;
    index = NULL;
}

Chord::~Chord(){
	Peer *delPtr = index->getSuccessor();
	Peer *tmp;
	do{
		tmp = delPtr->getSuccessor();
		delete delPtr;
		delPtr = tmp;
	}while (delPtr != index);
	delete index;
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
        //delete allocated peers
		Peer *delPtr = index->getSuccessor();
		Peer *tmp;
		do{
			tmp = delPtr->getSuccessor();
			delete delPtr;
			delPtr = tmp;
		}while (delPtr != index);
		delete index;
    }
    
    //new node at 0
    index = new Peer(0, this->chordSize);
    index->setSuccessor(index);
    
    //TODO: update finger table
}

void Chord::AddPeer(unsigned int id){
    if (id > pow(2, this->chordSize)) {
        throw std::string("AddPeer: ID for new peer is out of range for Chord size");
    }
    
    if (this->index == NULL) {
        throw std::string("AddPeer: Chord not initialised");
    }
    
    Peer *current = index;
    Peer *prev = index;
    
	if (current->getSuccessor() != current) {
		//not a zero element situation
		do {
			prev = current;
			current = current->getSuccessor();
		} while ((current->getID() < id) && (current != index));
	}

    Peer *insert = new Peer(id, this->chordSize);
    insert->setSuccessor(prev->getSuccessor());
    prev->setSuccessor(insert);
    
    //TODO: update finger table
	this->UpdateAllFingerTables();

	std::cout << "PEER " << id << " ADDED" << std::endl;
}

void Chord::RemovePeer(unsigned int id){
    if (id > pow(2, this->chordSize)) {
        throw std::string("RemovePeer: ID for new peer is out of range for Chord size");
    }
    
    if (this->index == NULL) {
        throw std::string("RemovePeer: Chord not initialised");
    }
    
    Peer *current = index->getSuccessor();
    Peer *prev = index;
    
    while ((current->getID() != id) && current->getSuccessor() != index->getSuccessor()) {
        prev = current;
        current = current->getSuccessor();
    }
    
    Peer *next = current->getSuccessor();
    
    //move data
    next->addData(current->getNodeData());
    
    prev->setSuccessor(next);

	//check if index* needs to be updated
	if (current == index) {
		index = next;
	}

	delete current;
    
    //TODO: update finger table
	this->UpdateAllFingerTables();

	std::cout << "PEER " << id << " REMOVED" << std::endl;
}

void Chord::FindKey(unsigned int key, Peer *foundPeer){

	if (key > pow(2, this->chordSize)) {
		throw std::string("FindKey: ID for search key is out of range for Chord size");
	}

	if (this->index == NULL) {
		throw std::string("FindKey: Chord not initialised");
	}

	//foundPeer is optional, NULL by default
	//search always starts at index
	Peer *ptr = index;
	bool found = false;

	//special case if index is being searched
	if (key == ptr->getID()) {
		found = true;
	}

	while (!found && (ptr->getSuccessor() != index)) {
		//look through finger table and select node to jump to
		std::vector<Peer*> &ft = ptr->getFingerTable();
		//finger table is going to be ordered asc
		Peer *jumpKey = ptr->getSuccessor();
		for (int i = 0; i < ft.size(); ++i) {
			if (ft[i]->getID() <= key){
				//max left
				jumpKey = ft[i];
			}
			if (ft[i]->getID() > key) {
				//too right
				break;
			}
		}
		std::cout << ptr->getID() << ">";
		ptr = jumpKey;
		if (jumpKey->getID() == key || (jumpKey->getID() > key)) {
			found = true;
		}
	}
	foundPeer = ptr;
	std::cout << foundPeer->getID();
	if (!found) {
		foundPeer = foundPeer->getSuccessor();
		std::cout << ">" << foundPeer->getID() << std::endl;
	}
	std::cout << std::endl;
}

void Chord::UpdateAllFingerTables(){
	Peer *cur = index;
	do{
		cur->updateFingerTable();
		cur = cur->getSuccessor();
	} while(cur != index);
}
