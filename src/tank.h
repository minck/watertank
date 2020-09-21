#ifndef TANK_H_
#define TANK_H_

typedef struct UA_Server UA_Server;
typedef struct UA_HistoryDataGathering UA_HistoryDataGathering;

void addTankObject(UA_Server *server, UA_HistoryDataGathering *gathering, float *size, float *value);

#endif
