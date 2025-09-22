#include <minix/ipc.h>
#include <minix/endpoint.h>
#include "calc.h"

int calc_multiply(endpoint_t calc_server, int a, int b, int *result)
{
    message msg;
    int r;

    msg.m_type = CALC_MUL_REQUEST;
    msg.m1_i1 = a;
    msg.m1_i2 = b;

    r = sendrec(calc_server, &msg);
    if (r != OK) return r;

    if (msg.m1_i2 != 0) return msg.m1_i2;  // server error

    *result = msg.m1_i1;
    return OK;
}
