#ifndef VALVE_H_
#define VALVE_H_

typedef struct UA_Server UA_Server;

void addValveObject(UA_Server *server, char *name, float *flow, float *value);

#endif
