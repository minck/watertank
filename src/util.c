#include "util.h"

UA_StatusCode readFloatDataSourceVariable(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    float *ctx = (float*)nodeContext;
    UA_Variant_setScalarCopy(&dataValue->value, ctx,
                             &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode writeFloatDataSourceVariable(UA_Server *server,
                 const UA_NodeId *sessionId, void *sessionContext,
                 const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    float *value = (float*)data->value.data;
    float *ctxValue = (float*)nodeContext;
    *ctxValue = *value;
    return UA_STATUSCODE_GOOD;
}
