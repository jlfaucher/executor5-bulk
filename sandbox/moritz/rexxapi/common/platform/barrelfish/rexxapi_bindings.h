/*
 * rexxapi_bindings.h
 *
 *  Created on: Mar 31, 2015
 *      Author: moritz
 */

#ifndef REXXAPI_COMMON_PLATFORM_BARRELFISH_REXXAPI_BINDINGS_H_
#define REXXAPI_COMMON_PLATFORM_BARRELFISH_REXXAPI_BINDINGS_H_

#include <sys/cdefs.h>

__BEGIN_DECLS

struct rexx_connection;

struct rexx_connection *rexx_connect(void);
void rexx_disconnect(struct rexx_connection *connection);

int rexx_read(struct rexx_connection *connection, uint8_t *buffer, size_t expected, size_t *actual);
int rexx_write(struct rexx_connection *connection, uint8_t *buffer, size_t expected, size_t *actual);

struct rexx_server;
struct rexx_client;

struct rexx_server *rexx_export(void);
void rexx_close(struct rexx_server *server);

struct rexx_client *rexx_accept(struct rexx_server *server);
void rexx_client_close(struct rexx_client *client);

int rexx_server_read(struct rexx_client *client, uint8_t *buffer, size_t expected, size_t *actual);
int rexx_server_write(struct rexx_client *client, uint8_t *buffer, size_t expected, size_t *actual);

__END_DECLS



#endif /* REXXAPI_COMMON_PLATFORM_BARRELFISH_REXXAPI_BINDINGS_H_ */
