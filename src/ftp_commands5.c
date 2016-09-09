/*
** ftp_commands5 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:05
** Last update on 14/05/16 18:05
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "commands.h"

bool                    ftp_pasv_check_failed(t_client *client)
{
    if (client != NULL)
    {
        if (client->port != NULL)
            free(client->port);
        client->port = NULL;
        FTP_RESPONSE_LINE(client->stream,
                          FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't open data channel.");
        return (false);
    }
    return (false);
}

bool                    ftp_pasv_no_port_found(t_client *client)
{
    if (client != NULL)
    {
        close(client->dts_server);
        if (client->port != NULL)
            free(client->port);
        client->port = NULL;
        FTP_RESPONSE_LINE(client->stream,
                          FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't open data channel.");
    }
    return (false);
}

bool                    ftp_pasv_listen(t_client *client)
{
    if (client == NULL)
        return (false);
    if (listen(client->dts_server, 4) < 0)
    {
        close(client->dts_server);
        if (client->port != NULL)
            free(client->port);
        client->port = NULL;
        FTP_RESPONSE_LINE(client->stream,
                          FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't open data channel.");
        return (false);
    }
    return (true);
}

bool                    ftp_pasv_init(t_client *client)
{
    struct sockaddr_in  *peer;
    unsigned short      port;

    port = 6666;
    client->dts_mode = FTP_DTS_PASSIVE;
    peer = (struct sockaddr_in *)(&(client->dts_client));
    peer->sin_family = AF_INET;
    peer->sin_addr.s_addr = htonl(INADDR_ANY);
    peer->sin_port = htons(port);
    while (bind(client->dts_server, (struct sockaddr *)peer,
                sizeof(struct sockaddr_in)) < 0)
    {
        if (port == 65535)
            return (ftp_pasv_no_port_found(client));
        port++;
        peer->sin_port = htons(port);
    }
    if (!ftp_pasv_listen(client))
        return (false);
    fprintf(client->stream,
            "%d Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n",
            FTP_RESPONSE_PASSIVE_MODE, 127, 0, 0, 1, (port & 0xFF00) >> 8,
            port & 0xFF);
    return (true);
}

bool                    ftp_pasv(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if ((client->dts_server = socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
        (client->port = malloc(sizeof(t_port))) == NULL)
        return (ftp_pasv_check_failed(client));
    return (ftp_pasv_init(client));
}