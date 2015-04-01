/** \file
 *  \brief Example rpc application using rpc stubs
 */

/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <stdio.h>
#include <string.h>

#include <barrelfish/barrelfish.h>
#include <barrelfish/nameservice_client.h>
#include <barrelfish/deferred.h>

#include <if/rexxapi_defs.h>
#include <if/rexxapi_rpcclient_defs.h>

#include "rexxapi_bindings.h"

const char *service_name = "rexxapi_service";

/* --------------------- Client ------------------------------ */

static struct rexxapi_rpc_client rexxapi_client;
//
//static void rexxapi_send(uint8_t *send_buffer, size_t send_buffer_size)
//{
//    errval_t err;
//
//    int in;
//    char *s_out;
//
//    debug_printf("client: sending myrpc\n");
//
//    in = 42;
//    err = rexxapi_client.vtbl.send(&rexxapi_client, in, &s_out);
//
//    if (err_is_ok(err)) {
//        debug_printf("client: myrpc(in: %u, out: '%s')\n", in, s_out);
//        free(s_out);
//    } else {
//        DEBUG_ERR(err, "xmlrpc myrpc");
//    }
//}

struct message {
    uint8_t *buffer;
    size_t size;
    size_t offset;
    struct message *next;
};

struct rexx_connection {
    struct message *messages;
    struct rexxapi_binding *binding;
    struct message pending_message;
};

struct rexx_client {
    struct rexxapi_binding *binding;
    struct message *messages;
    struct message pending_message;
};

struct rexx_server {
    iref_t iref;
    struct rexx_client *clients;
};

static inline void dispatch(void) {
    errval_t err;
    struct waitset * ws = get_default_waitset();
    err = event_dispatch_non_block(ws);
    if (err_no(err) == LIB_ERR_NO_EVENT) {
        barrelfish_usleep(80000);
    } else if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "%s: failed waiting for message.",
                __FUNCTION__);
    }
}

int rexx_read(struct rexx_connection *connection, uint8_t *buffer,
        size_t expected, size_t *actual) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);

    while (!connection->messages) {
        dispatch();
    }

    struct message *message = connection->messages;

    size_t remaining = message->size - message->offset;
    if (remaining <= expected) {
        connection->messages = message->next;
        memcpy(buffer, message->buffer, remaining);
        *actual = remaining;
        free(message->buffer);
        free(message);
    } else {
        memcpy(buffer, message->buffer, expected);
        *actual = expected;
        message->offset += expected;
    }
    return 1;
}

static void rexx_write_cb(void *st) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    struct rexx_connection *connection = (struct rexx_connection *) st;
    connection->pending_message.buffer = NULL;
}

static void rexx_write_ready(void *st) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    errval_t err;

    struct rexx_connection *connection = (struct rexx_connection *) st;

    struct event_closure txcont = MKCONT(rexx_write_cb, connection->binding);

    err = rexxapi_msg__tx(connection->binding, txcont,
            connection->pending_message.buffer,
            connection->pending_message.size);

    if (err_is_fail(err)) {
        DEBUG_ERR(err, "error sending msg_ints message\n");

        if (err_no(err) == FLOUNDER_ERR_TX_BUSY) {
            struct waitset *ws = get_default_waitset();
            txcont = MKCONT(rexx_write_ready, connection);
            err = connection->binding->register_send(connection->binding, ws,
                    txcont);
            if (err_is_fail(err)) {
                // note that only one continuation may be registered at a time
                DEBUG_ERR(err, "register_send on binding failed!");
            }
        }
    } else {
        debug_printf("%s:%s:%d send ok\n", __FILE__, __FUNCTION__, __LINE__);
    }
}

int rexx_write(struct rexx_connection *connection, uint8_t *buffer,
        size_t expected, size_t *actual) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);

    volatile struct message *msg = &connection->pending_message;
    msg->buffer = buffer;
    msg->size = expected;
    msg->offset = 0;

    rexx_write_ready(connection);

    while (msg->buffer) {
        dispatch();
    }
    *actual = expected;
    return 1;
}

static void client_rx_message(struct rexxapi_binding *b, uint8_t *buffer,
        size_t buffer_size) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);

    struct rexx_connection *connection = (struct rexx_connection *) b->st;

    struct message *message = calloc(1, sizeof(struct message));
    message->buffer = buffer;
    message->size = buffer_size;
    message->offset = 0;

    struct message *m = connection->messages;
    struct message *last = NULL;
    while (m) {
        last = m;
        m = m->next;
    }
    if (last == NULL) {
        // first message
        connection->messages = message;
    } else {
        last->next = message;
    }
}

static void bind_cb(void *st, errval_t err, struct rexxapi_binding *b) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "bind failed");
    }
    struct rexx_connection *connection = (struct rexx_connection *) st;

    rexxapi_rpc_client_init(&rexxapi_client, b);
    printf("client: finished rexxapi_rpc_client_init\n");

    b->rx_vtbl.msg = client_rx_message;
    b->st = st;

    connection->binding = b;
}

struct rexx_connection *rexx_connect(void) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    errval_t err;
    iref_t iref;

    err = nameservice_blocking_lookup(service_name, &iref);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "nameservice_blocking_lookup failed");
    }

    struct rexx_connection *connection = calloc(1, sizeof(struct rexx_connection));

    err = rexxapi_bind(iref, bind_cb, connection /* state for bind_cb */,
            get_default_waitset(),
            IDC_BIND_FLAGS_DEFAULT);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "bind failed");
    }

    // wait for bind_cb to return
    while (!connection->binding) {
        dispatch();
    }

    return connection;
}

void rexx_disconnect(struct rexx_connection *connection) {

    struct message *message;
    message = connection->messages;
    while (message) {
        struct message *next_message = message->next;
        free(message);
        message = next_message;
    }
    free(connection);
}


/* --------------------- Server ------------------------------ */
static void server_rx_message(struct rexxapi_binding *b, uint8_t *buffer,
        size_t buffer_size) {
    debug_printf("%s:%s:%d buffer=%p buffer_size=%zx\n", __FILE__, __FUNCTION__, __LINE__, buffer, buffer_size);

    struct rexx_client *client = b->st;

    struct message *message = calloc(1, sizeof(struct message));
    message->buffer = buffer;
    message->offset = 0;
    message->size = buffer_size;
    printf("%s:%s:%d message->size=%zx message->offset=%zx\n", __FILE__, __FUNCTION__, __LINE__, message->size, message->offset);

    struct message *m = client->messages;
    struct message *last = NULL;
    while (m) {
        last = m;
        m = m->next;
    }
    if (last == NULL) {
        // first message
        client->messages = message;
    } else {
        last->next = message;
    }

}


int rexx_server_read(struct rexx_client *client, uint8_t *buffer,
        size_t expected, size_t *actual) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    printf("client=%p client->messages=%p buffer=%p expected=%zx\n", client, client->messages, buffer, expected);

    while (!client->messages) {
        dispatch();
    }

    volatile struct message *message = client->messages;

    printf("%s:%s:%d message->size=%zx message->offset=%zx\n", __FILE__, __FUNCTION__, __LINE__, message->size, message->offset);
    size_t remaining = message->size - message->offset;
    printf("%s:%s:%d remaining=%zx\n", __FILE__, __FUNCTION__, __LINE__, remaining);
    if (remaining <= expected) {
        struct message *old = client->messages;
        client->messages = message->next;
        memcpy(buffer, message->buffer, remaining);
        *actual = remaining;
        free(message->buffer);
        free(old);
    } else {
        memcpy(buffer, message->buffer, expected);
        *actual = expected;
        message->offset += expected;
    }
    printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    return 1;
}

static void rexx_server_write_cb(void *st) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    struct rexx_client *client = (struct rexx_client *) st;
    client->pending_message.buffer = NULL;
}

static void rexx_server_write_ready(void *st) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    errval_t err;

    struct rexx_client *client= (struct rexx_client *) st;

    struct event_closure txcont = MKCONT(rexx_server_write_cb, client->binding);

    err = rexxapi_msg__tx(client->binding, txcont,
            client->pending_message.buffer,
            client->pending_message.size);

    if (err_is_fail(err)) {
        DEBUG_ERR(err, "error sending msg_ints message\n");

        if (err_no(err) == FLOUNDER_ERR_TX_BUSY) {
            struct waitset *ws = get_default_waitset();
            txcont = MKCONT(rexx_server_write_ready, client);
            err = client->binding->register_send(client->binding, ws,
                    txcont);
            if (err_is_fail(err)) {
                // note that only one continuation may be registered at a time
                DEBUG_ERR(err, "register_send on binding failed!");
            }
        }
    }
}

int rexx_server_write(struct rexx_client *client, uint8_t *buffer,
        size_t expected, size_t *actual) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);

    volatile struct message *msg = &client->pending_message;
    msg->buffer = buffer;
    msg->size = expected;
    msg->offset = 0;

    rexx_server_write_ready(client);

    while (msg->buffer) {
        dispatch();
    }
    *actual = expected;
    return 1;
}


//struct server_state {
//    struct rexxapi_binding *b;
//    char *s;
//};
//static void send_send_response(void *a)
//{
//    errval_t err;
//    struct server_state *st = (struct server_state*)a;
//
//    debug_printf("server: sending myresponse\n");
//
//    struct event_closure txcont = MKCONT(send_send_response_cb, st);
//    err = rexxapi_myrpc_response__tx(st->b, txcont, st->s);
//
//    if (err_is_fail(err)) {
//        if (err_no(err) == FLOUNDER_ERR_TX_BUSY) {
//            debug_printf("server: re-sending myresponse\n");
//            struct waitset *ws = get_default_waitset();
//            txcont = MKCONT(send_send_response, st);
//            err = st->b->register_send(st->b, ws, txcont);
//            if (err_is_fail(err)) {
//                // note that only one continuation may be registered at a time
//                DEBUG_ERR(err, "register_send on binding failed!");
//                free_st(st);
//            }
//        } else {
//            DEBUG_ERR(err, "error sending send message\n");
//            free_st(st);
//        }
//    }
//}
static struct rexxapi_rx_vtbl s_rx_vtbl = { .msg = server_rx_message };

static errval_t connect_cb(void *st, struct rexxapi_binding *b) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    struct rexx_server *server = (struct rexx_server *) st;

    struct rexx_client *client = calloc(1, sizeof(struct rexx_client));
    client->binding = b;
    client->messages = NULL;
    b->st = client;

    b->rx_vtbl = s_rx_vtbl;

    assert(server->clients == NULL);
    server->clients = client;

    return SYS_ERR_OK;
}

static void export_cb(void *st, errval_t err, iref_t iref) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    struct rexx_server *server = (struct rexx_server *) st;

    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "export failed");
    }

    err = nameservice_register(service_name, iref);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "nameservice_register failed");
    }

    server->iref = iref;
}

struct rexx_client *rexx_accept(struct rexx_server *server) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    volatile struct rexx_server *vserver = server;
    while (!vserver->clients) {
        dispatch();
    }

    struct rexx_client *client = server->clients;
    server->clients = NULL;
    return client;
}

struct rexx_server *rexx_export(void) {
    debug_printf("%s:%s:%d \n", __FILE__, __FUNCTION__, __LINE__);
    errval_t err;

    struct rexx_server *server = calloc(1, sizeof(struct rexx_server));
    server->iref = 0;

    err = rexxapi_export(
            server /* state pointer for connect/export callbacks */, export_cb,
            connect_cb, get_default_waitset(),
            IDC_EXPORT_FLAGS_DEFAULT);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "export failed");
    }

    volatile struct rexx_server *vserver = server;

    while (vserver->iref == 0) {
        dispatch();
    }

    return server;
}

void rexx_close(struct rexx_server *server) {
    free(server);
}


///* --------------------- Main ------------------------------ */
//
//int main(int argc, char *argv[]) {
//    errval_t err;
//
//    if ((argc >= 2) && (strcmp(argv[1], "client") == 0)) {
//        start_client();
//    } else if ((argc >= 2) && (strcmp(argv[1], "server") == 0)) {
//        start_server();
//    } else {
//        printf("usage: %s client|server\n", argv[0]);
//        return EXIT_FAILURE;
//    }
//
//    struct waitset *ws = get_default_waitset();
//    while (1) {
//        err = event_dispatch(ws);
//        if (err_is_fail(err)) {
//            DEBUG_ERR(err, "in event_dispatch");
//            break;
//        }
//    }
//
//    return EXIT_FAILURE;
//}

