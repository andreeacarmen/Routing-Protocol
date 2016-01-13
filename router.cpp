#include <iostream>
#include <vector>
#include "router.h"
#include "api.h"

/*Constructorul clasei */

Router::Router(int nr){
	vector<int> row;
	
	for (int i = 0 ; i<nr; i++)
	{
		for (int j = 0; j<nr; j++)
			row.push_back(-1);
		
		topology.push_back(row); //se initializeaza topologia ruterului
		routingTable.push_back(-1); //se initializeaza tabela de rutare
		msgVer.push_back(-1); //se intializeaza vectorul ce retine ultima versiune de mesaj primita

		row.clear();
	}
} 
