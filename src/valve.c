#include "valve.h"
#include "util.h"
#include <open62541/server.h>
#include <open62541/plugin/historydata/history_data_gathering.h>
#include <open62541/plugin/historydata/history_data_backend_memory.h>

extern UA_HistoryDataBackend backend;

static void addFlow(UA_Server *server, UA_NodeId parentId, float *value) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Flow");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId nodeId;
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "valve-flow");
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource timeDataSource;
    timeDataSource.read = readFloatDataSourceVariable;
    timeDataSource.write = writeFloatDataSourceVariable;
    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, parentId,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                        currentName,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                        attr,
                                        timeDataSource, value, &nodeId);
}

static void addValue(UA_Server *server, UA_HistoryDataGathering *gathering, UA_NodeId parentId, float *value) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Value");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
    attr.historizing = true;

    UA_NodeId nodeId;
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "valve-value");

    UA_DataSource timeDataSource;
    timeDataSource.read = readFloatDataSourceVariable;
    timeDataSource.write = writeFloatDataSourceVariable;
    UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, parentId,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), 
                                        currentName,
                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), 
                                        attr,
                                        timeDataSource, value, &nodeId);

    UA_HistorizingNodeIdSettings setting;
    //setting.historizingBackend = UA_HistoryDataBackend_Memory(1, 128);
    setting.historizingBackend = backend;
    setting.maxHistoryDataResponseSize = 100;
    setting.pollingInterval = 100;
    setting.historizingUpdateStrategy = UA_HISTORIZINGUPDATESTRATEGY_POLL;
    gathering->registerNodeId(server, gathering->context, &nodeId, setting);
    gathering->startPoll(server, gathering->context, &nodeId);
}

static UA_StatusCode openMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    float *value = (float*)methodContext;
    *value = 1.0f;
    return UA_STATUSCODE_GOOD;
}

static void addOpenMethod(UA_Server *server, UA_NodeId parentId, float *value) {
    UA_NodeId methodId;
    UA_MethodAttributes attr = UA_MethodAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US","Open");
    attr.executable = true;
    attr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            parentId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "valve-open-method"),
                            attr, &openMethodCallback,
                            0, NULL, 0, NULL, value, &methodId);
}

static UA_StatusCode closeMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    float *value = (float*)methodContext;
    *value = 0.0f;
    return UA_STATUSCODE_GOOD;
}

static void addCloseMethod(UA_Server *server, UA_NodeId parentId, float *value) {
    UA_NodeId methodId;
    UA_MethodAttributes attr = UA_MethodAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US","Close");
    attr.executable = true;
    attr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            parentId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "valve-close-method"),
                            attr, &closeMethodCallback,
                            0, NULL, 0, NULL, value, &methodId);
}

void addValveObject(UA_Server *server, UA_HistoryDataGathering *gathering, char *name, float *flow, float *value) {
    UA_NodeId valveId;
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, name), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &valveId);

    addFlow(server, valveId, flow);
    addValue(server, gathering, valveId, value);
    addOpenMethod(server, valveId, value);
    addCloseMethod(server, valveId, value);
}
