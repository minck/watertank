#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>

#include "tank.h"
#include "valve.h"

float tankSize = 2.0f;
float tankValue = 0.0f;

float valveInFlow = 0.08f;
float valveInValue = 0.0f;

float valveOutFlow = 0.08f;
float valveOutValue = 0.0f;

void updateStep(UA_Server *server, void *data)
{
    float s = 100.0f/1000.0f;

    tankValue += valveInFlow * valveInValue * s;
    tankValue -= valveOutFlow * valveOutValue * s;

    if(tankValue < 0.0f)
        tankValue = 0.0f;
    if(tankValue > tankSize)
        tankValue = tankSize;
}

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    addTankObject(server, &tankSize, &tankValue);
    addValveObject(server, "Valve In", &valveInFlow, &valveInValue);
    addValveObject(server, "Valve Out", &valveOutFlow, &valveOutValue);

    UA_Server_addRepeatedCallback(server, updateStep, NULL, 100, NULL);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}