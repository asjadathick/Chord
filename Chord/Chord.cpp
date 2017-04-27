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

	checkInit();
	checkKeyRange(id);
    
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

	checkInit();
	checkKeyRange(id);
    
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

void Chord::FindKey(unsigned int key){

	checkInit();
	checkKeyRange(key);

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

	std::cout << ptr->getID();
	if (!found) {
		ptr = ptr->getSuccessor();
		std::cout << ">" << ptr->getID() << std::endl;
	}
	std::cout << std::endl;
}

void Chord::FindKey(unsigned int key, Peer *&foundPeer){

	checkInit();
	checkKeyRange(key);

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

			if (jumpKey->getID() == key || (jumpKey->getID() > key)) {
				found = true;
				break;
			}

			if (ft[i]->getID() > key) {
				//too right
				break;
			}
		}
		std::cout << ptr->getID() << ">";
		ptr = jumpKey;
	}
	foundPeer = ptr;
	std::cout << foundPeer->getID();
	if (!found) {
		foundPeer = foundPeer->getSuccessor();
		std::cout << ">" << foundPeer->getID() << std::endl;
	}
	std::cout << std::endl;
}

void Chord::Insert(std::string value){
	ChangeData(value, true);
}

void Chord::Delete(std::string value){
	ChangeData(value, false);
}

void Chord::ChangeData(std::string value, bool insert){
	checkInit();

	unsigned int key = hash(value, this->chordSize);
	Peer *insPeer = NULL;
	FindKey(key, insPeer);

	if (insPeer == NULL) {
		throw std::string("ChangeData: could not find a node to update value");
	}

	if (insert) {
		insPeer->addData(value);
	} else {
		insPeer->removeData(value);
	}

	std::cout << (insert ? "INSERTED " : "REMOVED ") << value << " (key=" << key << ") AT " << insPeer->getID() << std::endl;
}

void Chord::Print(unsigned int key){
	checkInit();
	checkKeyRange(key);

	Peer *search = NULL;
	FindKey(key, search);

	if (search == NULL) {
		throw std::string("Print: could not find a peer with the key");
	}

	std::cout << "DATA AT NODE " << key << ":" << std::endl;
	search->printNodeData();
	std::cout << "FINGER TABLE OF NODE " << key << ":" << std::endl;
	search->printFingerTable();
	
}

void Chord::Read(std::string filename){
	std::ifstream input(filename.c_str());
	if (!input.good()) {
		throw std::string("Read: File could not be opened for read");
	}

	std::string cmdBuffer;
	std::string cmd, data;
	while (getline(input, cmdBuffer)) {
		//trim comments
		trimCommentsAndTrailingSpace(cmdBuffer);

		//get cmd and param for exec
		std::size_t pos = cmdBuffer.find(' ');
		cmd = cmdBuffer.substr(0, pos);
		data = cmdBuffer.substr(pos + 1);

		//exec
		if (cmd == "initchord") {
			InitChord(std::stoi(data));
		} else if (cmd == "addpeer"){
			AddPeer(std::stoi(data));
		} else if (cmd == "removepeer"){
			RemovePeer(std::stoi(data));
		} else if (cmd == "insert"){
			Insert(data);
		} else if (cmd == "delete"){
			Delete(data);
		} else if (cmd == "print"){
			Print(std::stoi(data));
		} else{
			throw std::string("Read: Input file contains invalid command");
		}
	}

	input.close();
}

//----Helpers---

void Chord::trimCommentsAndTrailingSpace(std::string& in){
	std::size_t pos = in.find('#');
	in = in.substr(0, pos -1);
	pos = in.find_last_not_of(" \r\n\t");
	if (std::string::npos != pos)
		in.erase(pos + 1);
}

void Chord::checkInit(){
	if (this->index == NULL) {
		throw std::string("ChordException: Chord not initialised");
	}
}

void Chord::checkKeyRange(unsigned int key){
	if (key > pow(2, this->chordSize)) {
		throw std::string("ChordException: ID for search key is out of range for Chord size");
	}
}

void Chord::UpdateAllFingerTables(){
	Peer *cur = index;
	do{
		cur->updateFingerTable();
		cur = cur->getSuccessor();
	} while(cur != index);
}
