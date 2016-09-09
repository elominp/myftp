/*
** commands for my_ftp
**
** Made by pirou_g in /home/pirou_g/ClionProjects/my_ftp
** Login   <pirou_g@epitech.net>
**
** Started on     26/04/16 16:32
** Last update on 26/04/16 16:32
*/

#ifndef MY_FTP_COMMANDS_H
# define MY_FTP_COMMANDS_H

# include <stdlib.h>
# include <stdbool.h>
# include "telnet.h"

# define ASCII 'A'
# define NON_PRINT 'N'
# define TELNET_FORMAT_EFFECTORS 'T'
# define EBCDIC 'C'
# define IMAGE 'I'
# define BYTE_SIZE 'L'
# define FILE_STRUCTURE 'F'
# define RECORD_STRUCTURE 'R'
# define PAGE_STRUCTURE 'P'
# define STREAM 'S'
# define BLOC 'B'
# define COMPRESSED 'C'

# define FTP_RESPONSE_LINE(s, c, t) fprintf(s, "%d %s\r\n", c, t)
# define FTP_RESPONSE(s, c, t) fprintf(s, "%d-%s\r\n%d End\r\n", c, t, c)
# define FTP_RESPONSE_CODE(s, m, da, de) fprintf(s, "%d%d%d", m, da, de)

# define FTP_RESPONSE_PRELIMINARY_GOOD 1
# define FTP_RESPONSE_DEFINITELY_GOOD 2
# define FTP_RESPONSE_INTERMEDIATE_GOOD 3
# define FTP_RESPONSE_TEMPORARY_BAD 4
# define FTP_RESPONSE_DEFINITELY_BAD 5

# define FTP_RESPONSE_SYNTAX 0
# define FTP_RESPONSE_INFORMATION 1
# define FTP_RESPONSE_CONNECT 2
# define FTP_RESPONSE_AUTH 3

# define FTP_RESPONSE_COMMAND_ENDED 200
# define FTP_RESPONSE_SYNTAX_ERROR 500
# define FTP_RESPONSE_BAD_PARAMETERS 501
# define FTP_RESPONSE_UNUSED_COMMAND 202
# define FTP_RESPONSE_UNKNOWN_COMMAND 502
# define FTP_RESPONSE_BAD_COMMAND_SEQUENCE 503
# define FTP_RESPONSE_COMMAND_ARGUMENT_NOT_IMPLEMENTED 504

# define FTP_RESPONSE_CONTROL_MARK 110
# define FTP_RESPONSE_SYSTEM_STATUS 211
# define FTP_RESPONSE_DIRECTORY_STATUS 212
# define FTP_RESPONSE_FILE_STATUS 213
# define FTP_RESPONSE_HELP 214
# define FTP_RESPONSE_SYSTEM_TYPE_NAME 215
# define FTP_RESPONSE_SERVICE_AVAILABLE_SOON 120
# define FTP_RESPONSE_SERVICE_AVAILABLE_FOR_NEW_USER 220
# define FTP_RESPONSE_CONTROL_CHANNEL_CLOSED_BY_SERVICE 221
# define FTP_RESPONSE_SERVICE_NOT_AVAILABLE 421
# define FTP_RESPONSE_DATA_CHANNEL_ALREADY_OPENED 125
# define FTP_RESPONSE_DATA_CHANNEL_OPENED_NO_TRANSFER 225
# define FTP_RESPONSE_ERROR_DATA_CHANNEL 425
# define FTP_RESPONSE_CLOSE_DATA_CHANNEL 226
# define FTP_RESPONSE_CLOSE_DATA_CHANNEL_INTERRUPT_TRANSFER 426
# define FTP_RESPONSE_PASSIVE_MODE 227
# define FTP_RESPONSE_SESSION_OPENED 230
# define FTP_RESPONSE_SESSION_NOT_OPENED 530
# define FTP_RESPONSE_USERNAME_GOT_PASSWORD_ASKED 331
# define FTP_RESPONSE_USERNAME_ASKED 332
# define FTP_RESPONSE_USERNAME_ASKED_FOR_RECORD 532
# define FTP_RESPONSE_FILE_STATUS_CHECKED 150
# define FTP_RESPONSE_FILE_SERVICE_ENDED 250
# define FTP_RESPONSE_PATH_CREATED 257
# define FTP_RESPONSE_FILE_SERVICE_WAIT_INFORMATION 350
# define FTP_RESPONSE_FILE_UNUSABLE 450
# define FTP_RESPONSE_FILE_NOT_ACCESSIBLE 550
# define FTP_RESPONSE_SERVICE_INTERRUPTED_LOCAL_ERROR 451
# define FTP_RESPONSE_SERVICE_INTERRUPTED_UNKNOWN_PAGE_TYPE 551
# define FTP_RESPONSE_NOT_ENOUGH_DISK_SPACE 452
# define FTP_RESPONSE_LIMIT_QUOTA_REACHED 552
# define FTP_RESPONSE_SERVICE_INTERRUPTED_BAD_FILE_NAME 553

# define FTP_DTS_NONE 0
# define FTP_DTS_ACTIVE 1
# define FTP_DTS_PASSIVE 2

typedef struct      s_port
{
    int             h1;
    int             h2;
    int             h3;
    int             h4;
    int             p1;
    int             p2;
}                   t_port;

typedef struct      s_client
{
    int             fd;
    struct sockaddr address;
    socklen_t       len;
    FILE            *stream;
    char            *username;
    int             is_logged;
    char            *password;
    t_port          *port;
    int             dts_mode;
    int             data_fd;
    struct sockaddr dts_client;
    int             dts_server;
}                   t_client;

typedef bool (*ftp_func)(t_client *, char *);

typedef struct  command
{
    const char* key;
    ftp_func    func;
}               command;

bool        ftp_not_implemented(t_client *, char *);
bool        ftp_user(t_client *, char *);
bool        ftp_pass(t_client *, char *);
bool        ftp_quit(t_client *, char *);
bool        ftp_cwd(t_client *, char *);
bool        ftp_cdup(t_client *, char *);
bool        ftp_port(t_client *, char *);
bool        ftp_retr(t_client *, char *);
bool        ftp_list(t_client *, char *);
bool        ftp_syst(t_client *, char *);
bool        ftp_pwd(t_client *, char *);
bool        ftp_type(t_client *, char *);
bool        ftp_rmd(t_client *, char *);
bool        ftp_mkd(t_client *, char *);
bool        ftp_pasv(t_client *, char *);
bool        ftp_dele(t_client *, char *);
bool        ftp_nlst(t_client *, char *);
bool        ftp_stat(t_client *, char *);
bool        ftp_help(t_client *, char *);
bool        ftp_noop(t_client *, char *);
bool        ftp_stor(t_client *, char *);
bool        ftp_appe(t_client *, char *);
void        clean_client(t_client *);
bool        ftp_checks(t_client *, char *);
bool        ftp_check_dts(t_client *, char *);
bool        ftp_send_open(t_client *, char *);
bool        ftp_send_close(t_client *, char *);
bool        ftp_send_error_close(t_client *);
bool        ftp_send_open_connect_failed(t_client *);

void        my_ftp_exec(t_client *, char *, char *);
int         open_command_server(unsigned short int);
void        remove_telnet_eol(char *);

extern const command commands[33];

#endif /* MY_FTP_COMMANDS_H */
