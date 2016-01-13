/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#ifndef _SIM_H_
#define _SIM_H_

void init_sim(int argc, char **argv);
void clean_sim();

void trigger_events();
void process_messages();
void update_routing_table();

#endif /* _SIM_H_ */
