/*
** ftp_commands7 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:11
** Last update on 14/05/16 18:11
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#include "commands.h"

bool        ftp_mkd(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || mkdir(data, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                      "Directory created.");
    return (true);
}

bool        ftp_dele(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || remove(data) < 0)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_SERVICE_ENDED,
                      "File removed.");
    return (true);
}

bool                ftp_nlst(t_client *client, char *data)
{
    DIR             *dir;
    struct dirent   *file;
    FILE            *dts;

    (void)data;
    if (!ftp_checks(client, data) || !ftp_send_open(client, data))
        return (false);
    if (data == NULL || (dts = fdopen(client->data_fd, "rb+")) == NULL ||
        (dir = opendir((data != NULL && *data != '\0') ? data : ".")) == NULL)
    {
        ftp_send_error_close(client);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_STATUS_CHECKED,
                      "Here comes directory listing.");
    while ((file = readdir(dir)) != NULL)
        fprintf(dts, "%s\r\n", file->d_name);
    fflush(dts);
    ftp_send_close(client, data);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_CLOSE_DATA_CHANNEL,
                      "Directory send OK.");
    return (true);
}

bool        ftp_stat(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    FTP_RESPONSE(client->stream, FTP_RESPONSE_SYSTEM_STATUS,
                 "FTP server status:\r\n\tLogged in as ftp\r\n \t"
                 "No session bandwith limit\r\n \t"
                 "myftp 1.0.0 - Not compatible with moulishit");
    return (true);
}

bool        ftp_help(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    FTP_RESPONSE(client->stream, FTP_RESPONSE_HELP,
                 "The following commands are recognized.\r\n"
                 " ABOR ACCT ALLO APPE CDUP CWD DELE HELP LIST MKD\r\n"
                 " MODE NLST NOOP PASS PASV PORT PWD QUIT REINT RETR\r\n"
                 " RMD RNFR RNTO SITE SIZE SMNT STAT STOR STOU STRU\r\n"
                 " SYST TYPE USER");
    return (true);
}