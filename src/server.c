#include "../include/sdfE.h"

static int server_fd, client_fd;
static char server_buf[SDFE_SERVER_BUFSIZE] = {0};
static struct pollfd pfds[1];

int sdfe_server_start(void)
{
        int status, yes = 1;
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_storage their_addr;
        socklen_t sin_size;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if ((status = getaddrinfo(NULL, SDFE_SERVER_PORT, &hints, &servinfo)) != 0) {
                SDFE_PUSH_ERROR("getaddrinfo failed!");
                return 0;
        }

        for (p = servinfo; p != NULL; p = p->ai_next) {
                if ((server_fd = socket(p->ai_family, p->ai_socktype,
                                        p->ai_protocol)) == -1) {
                        perror("socket");
                        continue;
                }

                if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                               sizeof(yes)) == -1) {
                        SDFE_PUSH_ERROR("setsockopt failed!");
                        return SDFE_SERVER_REUSE_ERROR;
                }

                if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(server_fd);
                        perror("bind");
                        continue;
                }

                break;
        }

        freeaddrinfo(servinfo);

        if (p == NULL) {
                SDFE_PUSH_ERROR("server failed to bind!");
                return SDFE_SERVER_BIND_ERROR;
        }

        if (listen(server_fd, 1) == -1) {
                SDFE_PUSH_ERROR("server failed to listen!");
                return SDFE_SERVER_LISTEN_ERROR;
        }

        sin_size = sizeof(their_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&their_addr, &sin_size);
        if (client_fd == -1) {
                SDFE_PUSH_ERROR("server failed to accept!");
                return SDFE_SERVER_ACCEPT_ERROR;
        }

        pfds[0].fd = client_fd;
        pfds[0].events = POLLIN;
        return 0;
}

char *sdfe_server_get_message(void)
{
        return server_buf;
}

sdfe_server_code_t sdfe_server_recv(void)
{
        int numevents = poll(pfds, 1, 50);
        if (numevents) {
                memset(server_buf, 0, SDFE_SERVER_BUFSIZE * sizeof(*server_buf));
                ssize_t nread = recv(client_fd, server_buf, SDFE_SERVER_BUFSIZE, 0);
                if (nread <= 0) {
                        SDFE_PUSH_ERROR("client disconnected or an error occured!");
                        return SDFE_SERVER_CODE_END;
                } else {
                        server_buf[nread] = '\0';
                        if (strcmp(server_buf, "shader-refresh") == 0) {
                                return SDFE_SERVER_CODE_REFRESH_SHADER;
                        } else if (strcmp(server_buf, "timer-reset") == 0) {
                                return SDFE_SERVER_CODE_RESET_TIMER;
                        } else {
                                return SDFE_SERVER_CODE_LOAD_SHADER;
                        }
                }
        }
        return SDFE_SERVER_CODE_NONE;
}

void sdfe_server_close(void)
{
        close(server_fd);
        close(client_fd);
        server_fd = 0;
        client_fd = 0;
}
