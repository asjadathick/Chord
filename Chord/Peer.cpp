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


Peer::Peer(unsigned long id, unsigned short chordSize){
    this->id = id;
    this->chordSize = chordSize;
    this->successor = NULL;
}

//---Data---

void Peer::addData(std::string data){
    this->data.push_back(data);
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

//--Successor

Peer* Peer::getSuccessor(){
    return successor;
}

void Peer::setSuccessor(Peer *value){
    successor = value;
}

Peer::~Peer(){
    Peer *cur = this;
    Peer *next;
    while (cur->successor != NULL) {
        next = cur->successor;
        delete cur;
        cur = next;
    }
    this->successor = NULL;
}
