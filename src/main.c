/*
** main for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:19
** Last update on 14/05/16 18:19
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commands.h"
#include "loops.h"

void        my_child_ftp(t_client *client)
{
    char    buffer[1024];
    char    *data = NULL;

    if (client == NULL || (client->stream = fdopen(client->fd, "wb+")) == NULL)
        return;
    FTP_RESPONSE_LINE(client->stream,
                      FTP_RESPONSE_SERVICE_AVAILABLE_FOR_NEW_USER,
                      "Connected to my_ftp server");
    while (fgets(buffer, 1024, client->stream) != NULL)
    {
        remove_telnet_eol(buffer);
        FOR(int i = 0, i < 1024 && buffer[i] != '\0', i++)
        {
            if (buffer[i] == ' ')
            {
                buffer[i] = '\0';
                data = (i < 1023 && buffer[i + 1] != '\0') ? buffer + i + 1 :
                       NULL;
                i = 1025;
            }
        }
        END_FOR
        my_ftp_exec(client, buffer, data);
        fflush(client->stream);
    }
}

void                my_ftp_init_client(t_client *client)
{
    if (client == NULL)
        return;
    client->username = NULL;
    client->password = NULL;
    client->is_logged = 0;
    client->port = NULL;
    client->dts_mode = FTP_DTS_NONE;
}

void                my_ftp(int port, const char *root)
{
    t_client        client;
    int             command_socket;
    pid_t           child;

    if (root == NULL || chdir(root) < 0 ||
        (command_socket = open_command_server(port)) < 0)
        return;
    while (42)
    {
        if ((client.fd = accept(command_socket, &(client.address),
                                &(client.len))) < 0 ||
            (child = fork()) < 0)
            break;
        if (!child)
        {
            close(command_socket);
            my_ftp_init_client(&client);
            my_child_ftp(&client);
        }
        else
            close(client.fd);
    }
}

int     main(int argc, char **argv)
{
    if (argc > 2)
        my_ftp(atoi(argv[1]), argv[2]);
    return (EXIT_SUCCESS);
}