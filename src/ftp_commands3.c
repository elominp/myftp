/*
** ftp_commands3 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:01
** Last update on 14/05/16 18:01
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include "commands.h"

bool        ftp_quit(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    (void)data;
    FTP_RESPONSE_LINE(client->stream,
                      FTP_RESPONSE_CONTROL_CHANNEL_CLOSED_BY_SERVICE,
                      "See you soon.");
    clean_client(client);
    exit(0);
}

bool        ftp_cwd(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || data[0] == '\0' || data[0] == '\n' || data[0] == '\r')
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_BAD_PARAMETERS,
                          "Bad arguments");
    }
    else if (chdir(data) < 0)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Failed to change directory.");
    }
    else
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_SERVICE_ENDED,
                          "Directory successfully changed.");
    return (true);
}

bool        ftp_cdup(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || chdir("..") < 0)
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Failed to change directory.");
    else
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                          "Directory successfully changed.");
    return (true);
}

bool        ftp_syst(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    (void)data;
#ifdef __linux
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SYSTEM_TYPE_NAME,
                      "UNIX Type: L8");
#else
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SYSTEM_TYPE_NAME,
                      "Unknown type");
#endif
    return (true);
}

bool        ftp_pwd(t_client *client, char *data)
{
    char    *pwd;

    (void)data;
    if (!ftp_checks(client, data))
        return (false);
    if ((pwd = get_current_dir_name()) == NULL)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    fprintf(client->stream, "%d \"%s\"\r\n", FTP_RESPONSE_PATH_CREATED, pwd);
    return (true);
}