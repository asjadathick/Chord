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
	index->setPredecessor(index);
    
    //TODO: update finger table
	UpdateAllFingerTables();
}

void Chord::AddPeer(unsigned int id){

	checkInit();
	checkKeyRange(id);

	Peer *insert;
	FindKey(id, insert);

	if (insert->getID() == id) {
		throw std::string("AddPeer: Node with that ID already exists");
	}

	Peer *newNode = new Peer(id, this->chordSize);

	if ((insert->getID() > id)) {
		if ((insert->getPredecessor()->getID() > id)) {
			insert = insert->getPredecessor();
		}
		//new node before insert node (new node is smaller)
		if (insert == index) {
			//first node
			index = newNode;
		}
		newNode->setSuccessor(insert);
		newNode->setPredecessor(insert->getPredecessor());
		insert->getPredecessor()->setSuccessor(newNode);
		insert->setPredecessor(newNode);

	} else {
		//new node is bigger
		if (insert->getPredecessor()->getID() < id) {
			insert = insert->getPredecessor();
		}
		newNode->setSuccessor(insert->getSuccessor());
		newNode->setPredecessor(insert);
		insert->getSuccessor()->setPredecessor(newNode);
		insert->setSuccessor(newNode);
	}

    //TODO: update finger table
	this->UpdateAllFingerTables();

	std::cout << "PEER " << id << " ADDED" << std::endl;
}

void Chord::RemovePeer(unsigned int id){

	checkInit();
	checkKeyRange(id);

	Peer *delPtr = NULL;
	FindKey(id, delPtr);

	if (delPtr == NULL) {
		throw std::string("RemovePeer: Peer with the ID doesn't exist");
	}

	//move data
	delPtr->getSuccessor()->addData(delPtr->getNodeData());

	//break links
	delPtr->getPredecessor()->setSuccessor(delPtr->getSuccessor());
	delPtr->getSuccessor()->setPredecessor(delPtr->getPredecessor());

	//check if index needs to be updated
	if (delPtr == index) {
		index = delPtr->getSuccessor();
	}

	//delete node
	delete delPtr;

    //TODO: update finger table
	this->UpdateAllFingerTables();

	std::cout << "PEER " << id << " REMOVED" << std::endl;
}

void Chord::FindKey(unsigned int key, Peer *&foundPeer){

	checkInit();
	checkKeyRange(key);

	Peer *ptr = index;
	bool found = false;
	bool print;

	do{
		print = true;

		if ((ptr->getID() >= key) && (ptr->getPredecessor()->getID() < key)) {
			found = true;
			print = false;
			break;
		}

		//print visit
		if (print) {
			std::cout << ptr->getID() << ">";
		}


		//check if only 1 peer in chord?
		if (ptr->getSuccessor() == ptr || ptr->getID() == key) {
			break;
		}

		//check if it's a join position
		if (ptr->getID() > ptr->getSuccessor()->getID()) {
			ptr = ptr->getSuccessor();
			break;
		}

		//check matches
		if ((ptr->getID() >= key) && (ptr->getPredecessor()->getID() < key)) {
			found = true;
			print = false;
			break;
		} else {
			//look in ft for largest but less than key peer
			std::vector<Peer*> ft = ptr->getFingerTable();
			std::sort(ft.begin(), ft.end(), Peer::ascSort);
			Peer *tmp = NULL;
			for (int i = 0; i < ft.size(); ++i) {
				if ((ft[i]->getID() <= key) && (ft[i]->getID() >= ptr->getID())) {
					if (tmp == NULL) {
						tmp = ft[i];
					} else if (ft[i]->getID() > tmp->getID()){
						tmp = ft[i];
					}
				}
			}

			if (tmp == ptr) {
				ptr = ptr->getSuccessor();
				break;
			}

			if (tmp == NULL) {
				tmp = ptr->getSuccessor();
			}

			ptr = tmp;
			//or pass to successor if none found
		}
	} while (!found);

	//print last
	std::cout << ptr->getID() << std::endl;
	foundPeer = ptr;
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

	std::cout << (insert ? "INSERTED " : "REMOVED ") << value << " (key=" << key << ") " << (insert ? "AT" : "FROM") << " " << insPeer->getID() << std::endl;
}

void Chord::Print(unsigned int key){
	checkInit();
	checkKeyRange(key);

	Peer *search = NULL;
	FindKey(key, search);

	if (search == NULL) {
		throw std::string("Print: could not find a peer with the key");
	}

	std::cout << "DATA AT INDEX NODE " << key << ":" << std::endl;
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
