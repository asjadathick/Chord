//
//  Peer.hpp
//  Chord
//
//  Created by Asjad Athick on 26/4/17.
//  Student: 4970512
//  Compilation: g++
//  Copyright © 2017 asjad.io. All rights reserved.
//

#ifndef Peer_hpp
#define Peer_hpp

#include <vector>
#include <map>
#include <cmath>
#include <string>


class Peer{
private:
    unsigned int id;
    unsigned short chordSize;
    Peer *successor;
    std::vector<std::string> data;
    std::vector<Peer*> fingerTable;
    
public:
    Peer(unsigned int id, unsigned short chordSize);
    
    unsigned int getID();
    
    //data
    void addData(std::string data);
    void addData(std::vector<std::string> data);
    std::vector<std::string> getNodeData();
    bool removeData(std::string data);
    
    //successor
    Peer *getSuccessor();
    void setSuccessor(Peer *value);
    
    //finger table
	void updateFingerTable();
    
    ~Peer();
};

#endif /* Peer_hpp */
