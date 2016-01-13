/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#include "sim.h"
#include "router.h"
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <assert.h>
#include "api.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define pii pair<int, int>


using namespace std;

/*structura pentru un ruter direct conectat*/
struct connectedRouter{
	int neigh, cost;
};

/*structura pentru un mesaj rutabil*/
struct mesaj{
	int src, dst, time, tag;
	string msg;
};

/*structura pentru un mesaj de gestionare a algoritmului de rutare*/
struct rmesaj{
	int src, ver, noNeigh;
	struct connectedRouter neigh[0]; 
};

/*structura pentru un eveniment */
struct event{
	int r1,r2,type,cost,time;
};

vector< Router > routers; //vectorul de rutere
int nrOfRouters, nrOfMessages, nrOfEvents, times; 
vector< mesaj > _messages; //vectorul de mesaje
vector< event > events;  //vectorul de evenimente

void init_sim(int argc, char **argv) {	
	/*citirea datelor despre rutere */	
	ifstream topologyFile;
    	topologyFile.open (argv[1]);
    	topologyFile>>nrOfRouters; //numarul de rutere
	
	/* initializarea vectorului de rutere */
	routers.reserve(nrOfRouters);
	for (int i = 0; i < nrOfRouters; i++){
		Router r = Router(nrOfRouters);
		routers.push_back(r);
	}
	
	/*se adauga in topologia fiecarui ruter vecinii direct conectati */
	int r1,r2,c;
	while(topologyFile>>r1>>c>>r2){
		routers[r1].topology[r1][r2] = routers[r1].topology[r2][r1] = c;
		routers[r2].topology[r2][r1] = routers[r2].topology[r1][r2] = c;	
	}	

	topologyFile.close();

	/*floodare initiala */
	for (int i = 0; i< nrOfRouters; i++)
	{
		vector< vector <int> > top = routers[i].topology;

		/*construire mesaj cu toate ruterele direct conectate*/
		int noNeigh = 0; //numarul de vecini direct conectati	
		for (int j = 0; j< nrOfRouters; j++)
			if(top[i][j] != -1)
				noNeigh++;

		/*alocare spatiu pentru mesaj */
		int message_size = sizeof(struct rmesaj) + noNeigh * sizeof(struct connectedRouter);
		struct rmesaj *message = (struct rmesaj *)calloc(1, message_size);
		assert(message != NULL);

		message->src = i;

		for (int j = 0; j< nrOfRouters; j++)
			if(top[i][j] != -1)			
			{
				message->neigh[message->noNeigh].neigh = j;
				message->neigh[message->noNeigh++].cost = top[i][j];
			}

		/*trimiterea mesajului catre toate ruterele direct conectate */
		for (int j = 0; j< nrOfRouters; j++)
			if(top[i][j] != -1) //daca exista legatura directa intre ruterul i si ruterul j
				endpoint[i].send_msg(&endpoint[j], (char*)message, message_size, (char *)"debug");

		free(message);
	}

	/* citire mesaje rutabile */
	ifstream messagesFile;
	messagesFile.open(argv[2]);
	
	int no_messages;
	
	messagesFile >> no_messages;

	nrOfMessages = no_messages;
	while(no_messages > 0){
		struct mesaj message;

		messagesFile >> message.src >> message.dst >> message.time >> message.tag;
		
		getline(messagesFile, message.msg);
		message.msg.append("\n");

		_messages.push_back(message);

		no_messages--;
	}

	messagesFile.close();

	/* citire evenimente */
	ifstream eventsFile;
    	eventsFile.open (argv[3]);
	eventsFile >> times;	
	eventsFile >> nrOfEvents;
	int no_events = nrOfEvents;

	while (no_events > 0){
		struct event ev;
		eventsFile >> ev.r1 >> ev.r2 >> ev.type >> ev.cost >> ev.time;
		events.push_back(ev);

		no_events --;	
	}
	
	eventsFile.close();
}

void clean_sim()
{
}

void trigger_events() {
	
	/*iterare prin vectorul de evenimente */
	for (int i = 0; i < nrOfEvents; i++)
		{	/*daca timpul evenimentului este acelasi cu timpul simulatorului, atunci acesta este declansat */
			if(events[i].time == get_time())
			{
				if (events[i].type == 0 ) //legatura noua
				{
					int r1 = events[i].r1;
					int r2 = events[i].r2;
					int cost = events[i].cost;
					
					/*se updateaza topologia ruterelor implicate */
					routers[r1].topology[r1][r2] = cost;
					routers[r1].topology[r2][r1] = cost;
					routers[r2].topology[r1][r2] = cost;
					routers[r2].topology[r2][r1] = cost;
				}
				else //legatura stearsa
					{
					int r1 = events[i].r1;
					int r2 = events[i].r2;

					/*se updateaza topologia ruterelor implicate */
					routers[r1].topology[r1][r2] = -1;
					routers[r1].topology[r2][r1] = -1;
					routers[r2].topology[r1][r2] = -1;
					routers[r2].topology[r2][r1] = -1;
					}
			}
		}	
		
	
	/*Dupa ce au fost declansate toate evenimentele de la timpul curent, cele 2 rutere implicate floodeaza reteaua cu mesaje care contin vecinii lor direct conectati si costurile aferente */

	for (int i = 0; i < nrOfEvents; i++)
		{
			if(events[i].time == get_time())
			{
				int r1 = events[i].r1;
				int r2 = events[i].r2;

				vector< vector <int> > top1 = routers[r1].topology;
				vector< vector <int> > top2 = routers[r2].topology;

				/*construirea mesajelor ce contin vecinii direct conectati ai celor 2 rutere implicate in eveniment */
				int noNeigh1 = 0, noNeigh2 = 0;	
				for (int j = 0; j< nrOfRouters; j++)
				{
					if(top1[r1][j] != -1)
						noNeigh1++;
					if(top2[r2][j] != -1)
						noNeigh2++;
				}

				unsigned int message_size1 = sizeof(struct rmesaj) + noNeigh1 * sizeof(struct connectedRouter);
				struct rmesaj *message1 = (struct rmesaj *)calloc(1, message_size1);
				assert(message1 != NULL);
			
				unsigned int message_size2 = sizeof(struct rmesaj) + noNeigh2 * sizeof(struct connectedRouter);
				struct rmesaj *message2 = (struct rmesaj *)calloc(1, message_size2);
				assert(message2 != NULL);

				message1->src = r1;
				message2->src = r2;
				message1->ver = get_time(); //se initializeaza versiunea mesajului ce urmeaza a fi trimis
				message2->ver = get_time();

				for (int j = 0; j< nrOfRouters; j++)
				{
					if(top1[r1][j] != -1)			
					{
						message1->neigh[message1->noNeigh].neigh = j;
						message1->neigh[message1->noNeigh++].cost = top1[r1][j];
					}
					if(top2[r2][j] != -1)			
					{
						message2->neigh[message2->noNeigh].neigh = j;
						message2->neigh[message2->noNeigh++].cost = top2[r2][j];
					}
				}

				/*trimiterea mesajelor catre toti vecinii direct conectati*/
				for (int j = 0; j< nrOfRouters; j++)
				{
					if(top1[r1][j] != -1) //daca ruterul i si ruterul j sunt conectati
					{
						printf("router %d sending pmessage to %d\n", r1, j);	 
						endpoint[r1].send_msg(&endpoint[j], (char*)message1, message_size1, (char *)"debug");
					}
					if(top2[r2][j] != -1) //daca i si j sunt conectate
					{
						printf("router %d sending pmessage to %d\n", r2, j);
						endpoint[r2].send_msg(&endpoint[j], (char*)message2, message_size2, (char *)"debug");
					}
					
				}
				/*eliberare memorie alocata */
				free(message1);
				free(message2);
			}
		}
}

void process_messages() {

	/*alocare spatiu pentru bufferele in care se vor primi mesajele */
	char buffer2[1024];
	unsigned int buffer_size = sizeof(struct rmesaj) + nrOfRouters * sizeof(struct connectedRouter);
	char *buffer = (char *)calloc(1, buffer_size);
	assert(buffer != NULL);

	/*se itereaza prin vectorul de mesaje */
	for (int j = 0; j < nrOfMessages; j++)
		{
			/*daca timpul mesajului coincide cu timpul simulatorului, atunci acesta este trimis */
			if(_messages[j].time == get_time())
			{
				int src = _messages[j].src;
				int dst = _messages[j].dst;
				int next_hop = routers[src].routingTable[dst];

				if(next_hop != -1)
					endpoint[src].route_message(&endpoint[next_hop], dst, _messages[j].tag, (char*)_messages[j].msg.c_str(), (char *)"debug");
			}
		}

	/*se itereaza prin vectorul de rutere si se verifica daca acestea au mesaje de primit */
	for (int i = 0; i < nrOfRouters; i++)
	{
		/*primirea mesajelor de gestionare a algoritmului de rutare */
		while(1)
		{
			int message_size = endpoint[i].recv_protocol_message(buffer);
			if(message_size == -1) //daca ruterul i nu mai are mesaje de primit
				break;

			struct rmesaj *message = (struct rmesaj *)buffer;
			
			/*daca ruterul i are o versiune mai noua a mesajului primit, atunci acesta este ignorat */ 
			if(message->ver <= routers[i].msgVer[message->src])
				continue;
			routers[i].msgVer[message->src] = message->ver;
			
			/*updatare topologie pe baza informatiilor din mesajul primit */
			for(int j = 0; j < nrOfRouters; j++)
			{
				routers[i].topology[message->src][j] = -1;
				routers[i].topology[j][message->src] = -1;
			}

			for(int j = 0; j < message->noNeigh; j++)
			{
				routers[i].topology[message->src][message->neigh[j].neigh] = message->neigh[j].cost;
				routers[i].topology[message->neigh[j].neigh][message->src] = message->neigh[j].cost;
			}
	
			/*forwarding la mesajul primit catre toti vecinii direct conectati */
			for (int j = 0; j< nrOfRouters; j++)
				if(routers[i].topology[i][j] != -1 && j != i && j != message->src) //daca i si j sunt conectate
					endpoint[i].send_msg(&endpoint[j], (char*)message, message_size, (char *)"debug");
		}
		/*primirea mesajelor rutabile*/
		while(1)
		{
			int src, dst, tag;
			bool received = endpoint[i].recv_message(&src, &dst, &tag, buffer2);
			if(received == false) //daca ruterul i nu mai are mesaje de primit
				break;
			
			/*daca ruterul i nu reprezinta destinatia mesajului, atunci acesta este trimit next hop-ului */
			if (i != dst && routers[i].routingTable[dst] != -1)
				endpoint[i].route_message(&endpoint[routers[i].routingTable[dst]], dst, tag, buffer2, (char *)"debug");
		}
  	}
}

void update_routing_table() {
	vector< vector<int> > d;
	vector< vector<int> > hop;

	/*calculul tabelei de routare pentru fiecare router, folosind algoritmul Roy–Floyd */ 
	for(int l = 0; l < nrOfRouters; l++)
	{
		for (int i = 0; i < nrOfRouters; i++)
		{
			vector<int> drow;
			vector<int> hoprow;

			for (int j = 0; j < nrOfRouters; j++)
			{
				if (routers[l].topology[i][j] == -1)
				{
					drow.push_back(32000);
					hoprow.push_back(-1);
				}
				else
				{
					drow.push_back(routers[l].topology[i][j]);
					hoprow.push_back(j);
				}
			}

			d.push_back(drow);
			hop.push_back(hoprow);
		}

		for(int i = 0; i < nrOfRouters; i++)
			for (int j = 0; j < nrOfRouters; j++)
				for(int k = 0; k < nrOfRouters; k++)
					if(d[i][j] > d[i][k] + d[k][j])
					{
						d[j][i] = d[i][j] = d[i][k] + d[k][j];
						hop[i][j] = hop[i][k];
						hop[j][i] = hop[j][k];
					}
					else if(d[i][j] == d[i][k] + d[k][j] && hop[i][j] > hop[i][k]){
						hop[i][j] = hop[i][k];
						hop[j][i] = hop[k][i];
						}

		for (int i = 0; i < nrOfRouters; i++)
			routers[l].routingTable[i] = hop[l][i]; //next hop;			
	}
}

