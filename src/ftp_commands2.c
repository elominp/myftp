/*
** ftp_commands2 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 17:57
** Last update on 14/05/16 17:57
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "commands.h"

void        clean_client(t_client *client)
{
    if (client == NULL)
        return;
    if (client->stream != NULL)
    {
        fflush(client->stream);
        fclose(client->stream);
    }
    if (client->username != NULL)
        free(client->username);
    if (client->password != NULL)
        free(client->password);
    close(client->fd);
    exit(0);
}

bool        ftp_checks(t_client *client, char *data)
{
    (void)data;
    if (client == NULL || client->stream == NULL)
        return (false);
    if (!client->is_logged)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SESSION_NOT_OPENED,
                          "Please login with USER and PASS.");
        return (false);
    }
    return (true);
}

bool        ftp_not_implemented(t_client *client, char *data)
{
    if (client == NULL || client->stream == NULL)
        return (false);
    (void)data;
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_UNKNOWN_COMMAND,
                      "Not implemented");
    return (true);
}

bool        ftp_user(t_client *client, char *data)
{
    if (client == NULL || client->stream == NULL)
        return (false);
    else if (data == NULL || !strcmp(data, " "))
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SESSION_NOT_OPENED,
                          "You need to log in with a user account. "
                                  "Anonymous connection is disabled but"
                                  " \"Anonymous\" account exist.");
        return (false);
    }
    if (client->username != NULL)
        free(client->username);
    if (client->password != NULL)
        free(client->password);
    client->password = NULL;
    if ((client->username = strdup(data)) == NULL)
        return (false);
    FTP_RESPONSE_LINE(client->stream,
                      FTP_RESPONSE_USERNAME_GOT_PASSWORD_ASKED,
                      "Please specify the password.");
    return (true);
}

bool        ftp_pass(t_client *client, char *data)
{
    if (client == NULL || client->stream == NULL)
        return (false);
    client->password = (data != NULL) ? strdup(data) : strdup("");
    if (!strcmp(client->username, "Anonymous"))
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SESSION_OPENED,
                          "Login successful.");
        client->is_logged = 1;
    }
    else
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SESSION_NOT_OPENED,
                          "Login incorrect.");
    }
    return (true);
}