//
//  Peer.cpp
//  Chord
//
//  Created by Asjad Athick on 26/4/17.
//  Student: 4970512
//  Compilation: g++
//  Copyright © 2017 asjad.io. All rights reserved.
//

#include "Peer.hpp"


Peer::Peer(unsigned int id, unsigned short chordSize){
    this->id = id;
    this->chordSize = chordSize;
    this->successor = NULL;
	this->fingerTable.resize(chordSize);
}

unsigned int Peer::getID(){
    return this->id;
}

//---Data---

void Peer::addData(std::string data){
    this->data.push_back(data);
}

void Peer::addData(std::vector<std::string> data){
    for (long i = 0; i < data.size(); ++i) {
        this->data.push_back(data[i]);
    }
}

std::vector<std::string> Peer::getNodeData(){
    return this->data;
}

bool Peer::removeData(std::string data){
    for (long i = 0; i < this->data.size(); ++i) {
        if (this->data[i] == data) {
            this->data.erase(this->data.begin() + i);
            return true;
        }
    }
    return false;
}

void Peer::printNodeData(){
	for (long i = 0; i < data.size(); ++i) {
		std::cout << data[i] << std::endl;
	}
}

//---Successor---

Peer* Peer::getSuccessor(){
    return successor;
}

void Peer::setSuccessor(Peer *value){
    this->successor = value;
}

//---Predecessor---

Peer* Peer::getPredecessor(){
	return predecessor;
}

void Peer::setPredecessor(Peer *value){
	this->predecessor = value;
}

//---Finger table---
void Peer::updateFingerTable(){
	//TODO: might need improvements/optimisations
	if (this->fingerTable.size() != this->chordSize) {
		this->fingerTable.resize(this->chordSize);
	}

	for (long i = 0; i < this->fingerTable.size(); ++i) {
		int upID = ((int)(this->id + pow(2, i)) % (int)(pow(2, chordSize)));
		Peer *ptr = successor;
		int hops = 0;
		while (ptr->getID() < upID && (hops < chordSize)) {
			ptr = ptr->successor;
			hops++;
		}
		fingerTable[i] = ptr;
	}
}

std::vector<Peer*>& Peer::getFingerTable(){
	return fingerTable;
}

void Peer::printFingerTable(){
	for (int i = 0; i < fingerTable.size(); ++i) {
		std::cout << fingerTable[i]->getID() << " ";
	}
	std::cout << std::endl;
}
