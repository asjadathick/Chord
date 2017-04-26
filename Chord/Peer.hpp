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


unsigned int hash(std::string input);

class Peer{
private:
    unsigned long id;
    unsigned short chordSize;
    Peer *successor;
    std::vector<std::string> data;
    std::map<int, Peer> fingerTable;
    
public:
    Peer(unsigned long id, unsigned short chordSize);
    
    //data
    void addData(std::string data);
    std::vector<std::string> getNodeData();
    bool removeData(std::string data);
    
    //successor
    Peer *getSuccessor();
    void setSuccessor(Peer *value);
    
    //finger table
};



#endif /* Peer_hpp */
