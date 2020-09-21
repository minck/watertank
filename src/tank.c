#include <open62541/server.h>
#include <open62541/plugin/historydata/history_data_gathering.h>
#include <open62541/plugin/historydata/history_data_backend_memory.h>
#include "util.h"

static UA_NodeId addSize(UA_Server *server, UA_NodeId parentId, float *size) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Size");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId nodeId;
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "tank-size");
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource timeDataSource;
    timeDataSource.read = readFloatDataSourceVariable;
    timeDataSource.write = writeFloatDataSourceVariable;
    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, parentId,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                        currentName,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                        attr,
                                        timeDataSource, size, &nodeId);

    return nodeId;
}

static UA_NodeId addValue(UA_Server *server, UA_HistoryDataGathering *gathering, UA_NodeId parentId, float *value) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Value");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
    attr.historizing = true;

    UA_NodeId nodeId;
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "tank-value");
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource timeDataSource;
    timeDataSource.read = readFloatDataSourceVariable;
    timeDataSource.write = writeFloatDataSourceVariable;
    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, parentId,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                        currentName,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                        attr,
                                        timeDataSource, value, NULL);

    UA_HistorizingNodeIdSettings setting;
    setting.historizingBackend = UA_HistoryDataBackend_Memory(3, 100);
    setting.maxHistoryDataResponseSize = 100;
    setting.pollingInterval = 1000;
    setting.historizingUpdateStrategy = UA_HISTORIZINGUPDATESTRATEGY_POLL;
    gathering->registerNodeId(server, gathering->context, &nodeId, setting);

    return nodeId;
}

void addTankObject(UA_Server *server, UA_HistoryDataGathering *gathering, float *size, float *value) {
    UA_NodeId tankId;
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Tank");
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "Tank"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &tankId);

    addSize(server, tankId, size);
    addValue(server, gathering, tankId, value);
}
