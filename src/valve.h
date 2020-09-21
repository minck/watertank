#ifndef VALVE_H_
#define VALVE_H_

typedef struct UA_Server UA_Server;
typedef struct UA_HistoryDataGathering UA_HistoryDataGathering;

void addValveObject(UA_Server *server, UA_HistoryDataGathering *gathering, char *name, float *flow, float *value);

#endif
