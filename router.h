#ifndef STUDENT_H
#define STUDENT_H

#include <vector>
#include <string>

using namespace std;


class Router {
	public:
	vector< int > routingTable; //tabela de rutare 
	vector< vector < int > > topology; //topologia
	vector< int > msgVer; //vector ce retine ultima versiune a mesajului primita de ruter
	
	Router(int nr); 
}; 

#endif
