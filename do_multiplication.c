static void handle_mul_request(message *msg, endpoint_t src)
{
    int r;
    int a = msg->m1_i1;
    int b = msg->m1_i2;
    long long result = (long long)a * (long long)b;  // avoid overflow

    printf("Calc server: received multiply request from %d: %d * %d\n", src, a, b);

    msg->m_type = CALC_MUL_REPLY;
    msg->m1_i1 = (int)result;  // return lower 32 bits
    msg->m1_i2 = 0;            // status = success

    r = send(src, msg);
    if (r != OK) {
        printf("Calc server: failed to send reply to %d: %d\n", src, r);
    } else {
        printf("Calc server: sent multiply reply to %d: result = %d\n", src, (int)result);
    }
}
