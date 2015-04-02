/*
 * rexxapi_test.c
 *
 *  Created on: Apr 1, 2015
 *      Author: moritz
 */

#include <stdio.h>
#include <string.h>

#include <barrelfish/barrelfish.h>
#include <barrelfish/deferred.h>

#include "rexxapi_bindings.h"

#define BUFFER_SIZE 0x258

int main(int argc, char **argv) {
	errval_t err;
    if (argc < 2) {
        debug_printf("Missing args, either server or client\n");
    } else if (strcmp("server", argv[1]) == 0) {

        struct rexx_server *server = rexx_export();
        struct rexx_client *client = rexx_accept(server);

        uint8_t buffer[BUFFER_SIZE];
        size_t actual;
        rexx_server_read(client, buffer, BUFFER_SIZE, &actual);

        debug_printf("received: %s\n", buffer);

    } else if (strcmp("client", argv[1]) == 0) {

        struct rexx_connection *connection = rexx_connect();

        uint8_t buffer[BUFFER_SIZE] = {'A', 'B', 'C', '\0' };

        size_t actual;

        rexx_write(connection, buffer, BUFFER_SIZE, &actual);

    }

	struct waitset *ws = get_default_waitset();
    while (1) {
    	err = event_dispatch(ws);
    	if (err_is_fail(err)) {
    		err_print_calltrace(err);
    		USER_PANIC_ERR(err, "dispatching failed");
    	}
    }
}
