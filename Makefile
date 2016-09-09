##
## Makefile for PSU_2015_myftp in /home/pirou_g/ClionProjects/PSU_2015_myftp
## 
## Made by pirou_g
## Login   <pirou_g@epitech.net>
## 
## Started on  Wed May 11 12:35:23 2016 pirou_g
## Last update Sat May 14 18:24:15 2016 pirou_g
##

CC	= gcc

CFLAGS	= -Wall -Wextra -std=gnu11 -I./include

LDFLAGS	=

SRCS	= src/main.c \
	  src/main2.c \
	  src/ftp_commands.c \
	  src/ftp_commands2.c \
	  src/ftp_commands3.c \
	  src/ftp_commands4.c \
	  src/ftp_commands5.c \
	  src/ftp_commands6.c \
	  src/ftp_commands7.c \
	  src/ftp_commands8.c

OBJS	= $(SRCS:.c=.o)

TMPS	= Makefile~ $(SRCS:.c=.c~)

RM	= rm -rf

BIN	= server

all:	$(BIN)

$(BIN):	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)

clean:
	$(RM) $(OBJS) $(TMPS)

fclean:	clean
	$(RM) $(BIN)

re:	fclean all

.PHONY:	all clean fclean re
