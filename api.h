/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#ifndef _API_H_
#define _API_H_

#include "endpoint.h"

using namespace std;

#define MAX_MSG_CONTENT 100
#define EVENT_TYPE_ADD 0
#define EVENT_TYPE_REMOVE 1

extern Endpoint *endpoint;

void init_api(int argc, char **argv);
void clean_api();

int get_time();

void api_update_state();

bool api_simulation_ended();

#endif /* _API_H_ */