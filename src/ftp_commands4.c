/*
** ftp_commands4 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:03
** Last update on 14/05/16 18:03
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "commands.h"

bool                    ftp_port_parsing_failed(t_client *client,
                                                t_port **pip,
                                                char *data,
                                                char *buf)
{
    t_port              *ip;

    if (client == NULL || pip == NULL || buf == NULL ||
        data == NULL ||
        (ip = (*pip = (client->port = malloc(sizeof(t_port))))) == NULL ||
        sscanf(data, "%d, %d, %d, %d, %d, %d", &(ip->h1), &(ip->h2), &(ip->h3),
               &(ip->h4), &(ip->p1), &(ip->p2)) < 6 || ip->h1 < 0 ||
        ip->h1 > 255 || ip->h2 < 0 || ip->h2 > 255 || ip->h3 < 0 ||
        ip->h3 > 255 || ip->h4 < 0 || ip->h4 > 255 ||
        snprintf(buf, 16, "%d.%d.%d.%d", ip->h1, ip->h2, ip->h3, ip->h4) < 0)
        return (true);
    return (false);
}

bool                    ftp_port_parsing_error(t_client *client)
{
    if (client != NULL)
    {
        if (client->port != NULL)
        {
            free(client->port);
            client->port = NULL;
        }
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SYNTAX_ERROR,
                          "Illegal PORT command. Format is h1, h2, "
                                  "h3, h4, p1, p2.");
        return (false);
    }
    return (false);
}

bool                    ftp_port_data_channel_error(t_client *client)
{
    if (client != NULL)
    {
        free(client->port);
        client->port = NULL;
        FTP_RESPONSE_LINE(client->stream,
                          FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't open data channel.");
        return (false);
    }
    return (false);
}

bool                    ftp_port(t_client *client, char *data)
{
    t_port              *ip;
    struct sockaddr_in  *peer;
    unsigned short      port;
    char                buffer[16];

    if (!ftp_checks(client, data))
        return (false);
    if (client->port != NULL)
        free(client->port);
    if (ftp_port_parsing_failed(client, &ip, data, buffer))
        return (ftp_port_parsing_error(client));
    port = (client->port->p1 << 8) + client->port->p2;
    client->dts_mode = FTP_DTS_ACTIVE;
    peer = (struct sockaddr_in *)(&(client->dts_client));
    peer->sin_port = htons(port);
    peer->sin_family = AF_INET;
    inet_aton(buffer, &(peer->sin_addr));
    if ((client->data_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return (ftp_port_data_channel_error(client));
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                      "PORT command successful. Consider using PASV if you"
                      " are behind a firewall.");
    return (true);
}