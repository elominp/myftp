/*
** ftp_commands for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     09/05/16 14:11
** Last update on 09/05/16 14:11
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "commands.h"

const command commands[33] = {{"USER", ftp_user},
                              {"PASS", ftp_pass},
                              {"ACCT", ftp_not_implemented},
                              {"CWD", ftp_cwd},
                              {"CDUP", ftp_cdup},
                              {"SMNT", ftp_not_implemented},
                              {"REINT", ftp_not_implemented},
                              {"QUIT", ftp_quit},
                              {"RMD", ftp_rmd},
                              {"MKD", ftp_mkd},
                              {"PWD", ftp_pwd},
                              {"SYST", ftp_syst},
                              {"PORT", ftp_port},
                              {"PASV", ftp_pasv},
                              {"TYPE", ftp_type},
                              {"STRU", ftp_not_implemented},
                              {"MODE", ftp_not_implemented},
                              {"RETR", ftp_retr},
                              {"STOR", ftp_stor},
                              {"STOU", ftp_not_implemented},
                              {"APPE", ftp_appe},
                              {"ALLO", ftp_not_implemented},
                              {"REST", ftp_not_implemented},
                              {"RNFR", ftp_not_implemented},
                              {"RNTO", ftp_not_implemented},
                              {"ABOR", ftp_not_implemented},
                              {"DELE", ftp_dele},
                              {"LIST", ftp_list},
                              {"NLST", ftp_nlst},
                              {"SITE", ftp_not_implemented},
                              {"STAT", ftp_stat},
                              {"HELP", ftp_help},
                              {"NOOP", ftp_noop},
};

bool                    ftp_check_dts(t_client *client, char *data)
{
    (void)data;
    if (client != NULL &&
        (client->dts_mode == FTP_DTS_NONE ||
         (client->dts_mode == FTP_DTS_ACTIVE && client->port == NULL) ||
         (client->dts_mode == FTP_DTS_PASSIVE && client->port == NULL)))
        return (true);
    return (false);
}

bool                    ftp_send_open_connect_failed(t_client *client)
{
    if (client != NULL)
    {
        close(client->dts_server);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't connect to client.");
    }
    return (false);
}

bool                    ftp_send_open(t_client *client, char *data)
{
    socklen_t           len;

    if (ftp_check_dts(client, data))
    {
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Use PORT or PASV first.");
        return (false);
    }
    if (client->dts_mode == FTP_DTS_ACTIVE &&
        connect(client->data_fd, &(client->dts_client),
                sizeof(client->dts_client)) < 0)
    {
        close(client->data_fd);
        FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_ERROR_DATA_CHANNEL,
                          "Can't connect to host.");
        return (false);
    }
    else if (client->dts_mode == FTP_DTS_PASSIVE &&
             (client->data_fd = accept(client->dts_server,
                                       &(client->dts_client), &len)) < 0)
        return (ftp_send_open_connect_failed(client));
    return (true);
}

bool                    ftp_send_close(t_client *client, char *data)
{
    (void)data;
    if (client->dts_mode == FTP_DTS_ACTIVE)
    {
        close(client->data_fd);
        if (client->port != NULL)
        {
            free(client->port);
            client->port = NULL;
        }
    }
    else if (client->dts_mode == FTP_DTS_PASSIVE)
    {
        close(client->data_fd);
        close(client->dts_server);
        if (client->port != NULL)
        {
            free(client->port);
            client->port = NULL;
        }
    }
    return (true);
}

bool                    ftp_send_error_close(t_client *client)
{
    if ((client->port != NULL))
    {
        free(client->port);
        client->port = NULL;
    }
    close(client->data_fd);
    if (client->dts_mode == FTP_DTS_PASSIVE)
        close(client->dts_server);
    return (true);
}