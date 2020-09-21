#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>

#include "tank.h"
#include "valve.h"
#include "perlin/perlin.h"

float tankSize = 2.0f;
float tankValue = 0.0f;

float valveInFlow = 0.08f;
float valveInValue = 0.0f;

float valveOutFlow = 0.08f;
float valveOutValue = 0.0f;

int step = 100; // in milliseconds
float t = 0.0f;

void updateStep(UA_Server *server, void *data)
{
    float s = step/1000.0f;
    t += s;

    valveOutValue = (pnoise1d(t / 10.0f, 1.0/4.0, 4, 567890) + 1) / 2.0f;

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

    UA_Server_addRepeatedCallback(server, updateStep, NULL, step, NULL);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}