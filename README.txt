Ifrim Andreea-Carmen
Grupa 322CB

Tema 2: Simularea unui Protocol de Routare

Pentru simularea acestui protocol de rutare, am implementat urmatoarele variabile, structuri si clase ajutatoare:

1. class Router - va reprezenta un ruter, fiecare ruter avand urmatoarele atribute:
	1.1 - vector< int > routingTable - tabela de rutare 
	1.2 - vector< vector < int > > topology - topologia
	1.3 - vector< int > msgVer - vector in care se va retine ultima versiune a mesajului primit
Constructorul clasei primeste ca parametru numarul de rutere din retea si va initializa corespunzator vectorii.

2. struct connectedRouter - structura pentru un ruter direct conectat, cu urmatoarele campuri:
	2.1 - neigh - id-ul vecinului direct conectat
	2.2 - cost - costul legaturii

3. struct mesaj - structura pentru un mesaj rutabil, ce are campuri pentru sursa (previous hop-ul), destinata, timpul, tagul si sirul de caractere continut de mesaj
	
4. struct rmesaj - structura pentru un mesaj de gestionare a algoritmului de rutare, cu urmatoarele campuri:
	4.1 src - sursa mesajului 
	4.2 ver	- versiunea mesajului 
	4.3 noNeigh - numarul de rutere continute in mesaj
	4.4 struct connectedRouter neigh[0] - vector in care se retin vecinii direct conectati ai ruterului ce va trimite mesajul
	
5. struct event - structura pentru un eveniment, ce are campuri pentru cele 2 rutere implicate, tipul legaturii, costul acesteia si timpul la care se declanseaza
evenimentul

6. vector< Router > routers - vectorul de rutere 
7. vector< mesaj > _messages; - vectorul de mesaje
8. vector< event > events - vectorul de evenimente 
9. Variabile globale pentru a retine numarul de rutere, numarul de mesaje si numarul de evenimente

De asemenea, pentru functiile din schelet am realizat urmatoarea implementare:

1. init_sim:
	Aceasta functie este folosita pentru citirea datelor din cele 3 fisiere de intrare si initializarea si completarea vectorilor de rutere, de mesaje si de evenimente. Pe baza datelor citite se incepe si completarea topologiei fiecarui ruter, trecandu-se doar vecinii direct conectati.
	De asemenea, se incepe floodarea initiala, fiecare ruter trimitand un mesaj de tipul rmesaj cu toti vecinii direct conectati.

2. trigger_events:
	Se itereaza prin vectorul de evenimente si se declanseaza doar cele al caror timp corespunde cu timpul simulatorului. La declansare, se verifica tipul legaturii si se modifica topologia celor doua rutere implicate corespunzator. 
	Dupa ce au fost declansate toate evenimentele de la timpul curent, cele doua rutere implicate in fiecare eveniment floodeaza reteaua cu mesaje (de tipul rmesaj - folosite la gestionarea algoritmului de rutare) care contin vecinii lor direct conectati si costurile aferente. De asemenea, versiunea mesajelor trimise se seteaza egala cu timpul simularii.

3. process_messages:
	3.1 Trimiterea mesajelor:
	Se itereaza prin vectorul de mesaje si se trimit doar cele al caror timp corespunde cu timpul simularii. Acestea vor fi trimise catre next hop-ul din tabela de rutare a sursei.
	3.2 Primirea mesajelor:
	Pentru fiecare ruter se verifica daca acesta are mesaje de primit. Ruterele pot primi atat mesaje rutabile, cat si mesaje folosite la gestionarea algoritmului de rutare.
	La primirea unui mesaj de rutare, se verifica daca versiunea acestuia este mai noua decat versiunea retinuta de ruter si in caz afirmativ, se updateaza topologia pe baza informatiilor continute si se trimite mai departe catre toti vecinii direct conectati. In caz contrar, mesajul este ignorat. 
	Mesajul este primit intr-un buffer de tipul char *, alocat dinamic, convertit apoi catre o structura de tipul rmesaj;
	La primirea unui mesaj rutabil, se verifica daca acesta a ajuns la destinatie (daca ruterul care a primit mesajul coincide cu destinatia acestuia) si in caz contrar, se trimite catre next hop.
	Mesajul este primit intr-un buffer de tipul char[], alocat static.

4. update_routing_table:
	Pentru determinarea tabelei de rutare, am folosit algoritmul Roy–Floyd. Acesta calculeaza calea de cost minim de intre oricare 2 rutere.
	Algoritmul este aplicat pentru fiecare ruter in parte (folosind informatiile din tabela acestuia de rutare).

Eliberarea memoriei se face corespunzator in fiecare functie mentionata. 
