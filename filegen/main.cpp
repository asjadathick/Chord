//
//  main.cpp
//  filegen
//
//  Created by Asjad Athick on 29/4/17.
//  Copyright © 2017 asjad.io. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

string RandomString(int len)
{
//	srand(time(0));
	string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string newstr;
	int pos;
	while(newstr.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos,1);
	}
	return newstr;
}

int main(int argc, const char * argv[]) {
	string filename = "/Users/Asjad/Documents/CSCI319/Chord/filegen/medFile.dat";
	ofstream out(filename.c_str());
	if (!out.good()) {
		cout << "Could not open file." << endl;
		return -1;
	}

	vector<int> addedPeers;
	addedPeers.push_back(0);

	vector<string> addedDatas;
	addedDatas.push_back("hey");
	long chordSize = pow(2, 32);

	out << "initchord 32" << endl;
	out << "addpeer 0" << endl;
	out << "insert hey" << endl;

	for (int i = 0; i < 1000000; ++i) {
		int action = rand() % 5;

		int pid = rand() % chordSize;
		int rid = rand() % addedPeers.size();
		int rdid = rand() % addedDatas.size();

		switch (action) {
			case 0:
				//add peer
				out << "addpeer " << pid << endl;
				break;
			case 1:
				//remove peer
				out << "removepeer " << rid << endl;
				break;
			case 2:
				//insert
				addedDatas.push_back(RandomString(15));
				out << "insert " << addedDatas.back() << endl;
				break;
			case 3:
				//delete
				out << "delete " << addedDatas[rdid] << endl;
				break;

			case 4:
				//print
				out << "print " << pid << endl;
		}
	}

	out.close();
	
	return 0;
}
