/*
 * calc - Calculator Server
 *
 * A simple system process that handles addition and multiplication requests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <minix/endpoint.h>
#include <minix/syslib.h>
#include <minix/ipc.h>
#include "calc.h"  /* Include your shared message type definitions */

/* Function prototypes */
static void handle_add_request(message *msg, endpoint_t src);
static void handle_mul_request(message *msg, endpoint_t src);

int main(int argc, char **argv)
{
    int r;
    endpoint_t src;
    message msg;

    printf("Calc server: starting up\n");

    /* Main message loop */
    while (1) {
        /* Wait for any incoming message */
        r = sef_receive(ANY, &msg, &src);

        if (r != OK) {
            printf("Calc server: sef_receive failed with %d\n", r);
            continue;
        }

        /* Handle message based on type */
        switch (msg.m_type) {
            case CALC_ADD_REQUEST:
                handle_add_request(&msg, src);
                break;

            case CALC_MUL_REQUEST:
                handle_mul_request(&msg, src);
                break;

            default:
                /* Unknown message type, send error reply */
                msg.m_type = CALC_ADD_REPLY;  /* default reply type */
                msg.m1_i1 = -1;               /* Error result */
                msg.m1_i2 = EINVAL;           /* Invalid argument error code */
                r = send(src, &msg);
                if (r != OK) {
                    printf("Calc server: failed to send error reply to %d\n", src);
                }
                break;
        }
    }

    return 0;  /* Should never reach here */
}

/* Handle an addition request */
static void handle_add_request(message *msg, endpoint_t src)
{
    int r;
    int a = msg->m1_i1;
    int b = msg->m1_i2;
    long long sum = (long long)a + (long long)b;

    printf("Calc server: received add request from %d: %d + %d\n", src, a, b);

    msg->m_type = CALC_ADD_REPLY;
    msg->m1_i1 = (int)sum;  /* result (lower 32 bits) */
    msg.m1_i2 = 0;          /* status: 0 = success */

    r = send(src, msg);
    if (r != OK) {
        printf("Calc server: failed to send add reply to %d: %d\n", src, r);
    } else {
        printf("Calc server: sent add reply to %d: result = %d\n", src, (int)sum);
    }
}

/* Handle a multiplication request */
static void handle_mul_request(message *msg, endpoint_t src)
{
    int r;
    int a = msg->m1_i1;
    int b = msg->m1_i2;
    long long product = (long long)a * (long long)b;

    printf("Calc server: received multiply request from %d: %d * %d\n", src, a, b);

    msg->m_type = CALC_MUL_REPLY;
    msg->m1_i1 = (int)product;  /* result (lower 32 bits) */
    msg->m1_i2 = 0;             /* status: 0 = success */

    r = send(src, msg);
    if (r != OK) {
        printf("Calc server: failed to send multiply reply to %d: %d\n", src, r);
    } else {
        printf("Calc server: sent multiply reply to %d: result = %d\n", src, (int)product);
    }
}
