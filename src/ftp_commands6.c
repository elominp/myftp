/*
** ftp_commands6 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:07
** Last update on 14/05/16 18:07
*/

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

bool                    ftp_retr(t_client *client, char *data)
{
    int                 fd;
    char                buffer[1024];
    ssize_t             len;

    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || (fd = open(data, O_RDONLY)) < 0)
    {
        ftp_send_error_close(client);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "File doesn't exist or is not accessible.");
        return (false);
    }
    if (!ftp_send_open(client, data))
        return (false);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_STATUS_CHECKED,
                      "Here comes the file.");
    while ((len = read(fd, buffer, 1024)) > 0)
        write(client->data_fd, buffer, len);
    close(fd);
    ftp_send_close(client, data);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_CLOSE_DATA_CHANNEL,
                      "File successfully transferred.");
    return (true);
}

bool        ftp_list_popen_failed(t_client *client)
{
    if (client != NULL)
    {
        ftp_send_error_close(client);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
    }
    return (false);
}

bool        ftp_list(t_client *client, char *data)
{
    FILE    *lst;
    char    buffer[512];
    size_t  len;
    char    *cmd;
    size_t  cmd_len;

    (void)data;
    cmd_len = (data != NULL) ? strlen(data) + 6 : 5;
    if (!ftp_checks(client, data) || (cmd = malloc(cmd_len + 1)) == NULL ||
        sprintf(cmd, "ls -l %s", data) < 1)
        return (false);
    if ((lst = popen(cmd, "r")) == NULL || WEXITSTATUS(pclose(lst)) != 141 ||
        (lst = popen(cmd, "r")) == NULL)
        return (ftp_list_popen_failed(client));
    else if (!ftp_send_open(client, data))
        return (ftp_send_open_connect_failed(client));
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_STATUS_CHECKED,
                      "Here comes the directory listing.");
    while ((len = fread(buffer, 1, 512, lst)) > 0)
        write(client->data_fd, buffer, len);
    pclose(lst);
    ftp_send_close(client, data);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_CLOSE_DATA_CHANNEL,
                      "Directory send OK.");
    return (true);
}

bool        ftp_type(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || (data[0] != 'I' && data[0] != 'A'))
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_BAD_PARAMETERS,
                          "Unrecognized option.");
        return (false);
    }
    if (data[0] == 'I')
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                          "Switching to binary mode.");
    }
    else if (data[0] == 'A')
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                          "Switching to ascii mode.");
    }
    return (true);
}

bool        ftp_rmd(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    if (data == NULL || rmdir(data) < 0)
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                      "Directory removed.");
    return (true);
}