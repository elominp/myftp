/*
** ftp_commands8 for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     14/05/16 18:13
** Last update on 14/05/16 18:13
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "commands.h"

bool        ftp_noop(t_client *client, char *data)
{
    if (!ftp_checks(client, data))
        return (false);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_COMMAND_ENDED,
                      "NOOP ok.");
    return (true);
}

bool        ftp_stor(t_client *client, char *data)
{
    int     fd;
    char    buffer[1024];
    ssize_t len;

    if (!ftp_checks(client, data) || !ftp_send_open(client, data))
        return (false);
    if (data == NULL ||
        (fd = open(data, O_WRONLY | O_TRUNC | O_CREAT,
                   S_IRWXU | S_IRWXG | S_IROTH)) < 0)
    {
        ftp_send_error_close(client);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_STATUS_CHECKED,
                      "Reading file data.");
    while ((len = read(client->data_fd, buffer, 1024)) > 0)
        write(fd, buffer, len);
    close(fd);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_CLOSE_DATA_CHANNEL,
                      "File stored.");
    ftp_send_close(client, data);
    return (true);
}

bool        ftp_appe(t_client *client, char *data)
{
    int     fd;
    char    buffer[1024];
    ssize_t len;

    if (!ftp_checks(client, data) || !ftp_send_open(client, data))
        return (false);
    if (data == NULL ||
        (fd = open(data, O_WRONLY | O_CREAT,
                   S_IRWXU | S_IRWXG | S_IROTH)) < 0 ||
        lseek(fd, 0, SEEK_END) < 0)
    {
        ftp_send_error_close(client);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_NOT_ACCESSIBLE,
                          "Permission denied.");
        return (false);
    }
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_FILE_STATUS_CHECKED,
                      "Reading file data.");
    while ((len = read(client->data_fd, buffer, 1024)) > 0)
        write(fd, buffer, len);
    close(fd);
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_CLOSE_DATA_CHANNEL,
                      "File stored.");
    ftp_send_close(client, data);
    return (true);
}