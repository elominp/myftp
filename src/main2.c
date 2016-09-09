/*
** main2 for my_ftp
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
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include "commands.h"
#include "loops.h"

void    my_clean(void)
{

}

void    my_command_sighandler(int signal)
{
    if (signal == SIGPIPE || signal == SIGQUIT || signal == SIGINT ||
        signal == SIGTERM)
        my_clean();
}

void            my_ftp_exec(t_client *client, char *command, char *data)
{
    ftp_func    func;

    if (client == NULL || command == NULL)
        return;
    FOR(int i = 0, i < 33, i++)
    {
        if (!strcmp(commands[i].key, command))
        {
            func = commands[i].func;
            if (func != NULL)
                func(client, data);
            return;
        }
    }
    END_FOR
    FTP_RESPONSE_LINE(client->stream, FTP_RESPONSE_SYNTAX_ERROR,
                      "Unrecognized command, please verify with help it "
                              "exist or you don't mispelled it");
}

int     open_command_server(unsigned short int port)
{
    int sock;
    struct sockaddr_in bind_data;

    bind_data.sin_family = AF_INET;
    bind_data.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_data.sin_port = htons(port);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ||
        bind(sock, (const struct sockaddr *)&bind_data,
             sizeof(struct sockaddr_in)) < 0 || listen(sock, 4) < 0)
        return (-1);
    signal(SIGPIPE, my_command_sighandler);
    return (sock);
}

void        remove_telnet_eol(char *buffer)
{
    if (buffer == NULL)
        return;
    while (*buffer != '\0')
    {
        if (*buffer == '\r' || *buffer == '\n')
        {
            *buffer = '\0';
            return;
        }
        buffer++;
    }
}