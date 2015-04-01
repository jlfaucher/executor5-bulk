/*
 * rexxapi_test.c
 *
 *  Created on: Apr 1, 2015
 *      Author: moritz
 */

#include <stdio.h>
#include <string.h>

#include <barrelfish/barrelfish.h>

#include "rexxapi_bindings.h"

#define BUFFER_SIZE 16

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Missing args, either server or client\n");
    } else if (strcmp("server", argv[1]) == 0) {

        struct rexx_server *server = rexx_export();
        struct rexx_client *client = rexx_accept(server);

        uint8_t buffer[BUFFER_SIZE];
        size_t actual;
        rexx_server_read(client, buffer, BUFFER_SIZE, &actual);

        printf("received: %s\n", buffer);

    } else if (strcmp("client", argv[1]) == 0) {

        struct rexx_connection *connection = rexx_connect();

        uint8_t buffer[] = {'A', 'B', 'C', '\0' };

        size_t actual;

        rexx_write(connection, buffer, 4, &actual);

    }
}
