# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/23 14:34:00 by sumjang           #+#    #+#              #
#    Updated: 2022/09/23 18:05:09 by sjo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = minishell
CFLAGS	=	-Wall -Wextra -Werror -fsanitize=address
LIBFT_DIR = ./libft/
PARSE_DIR = ./parse/
BUILTIN_DIR = ./builtins/
UTIL_DIR = ./utils/
EXEC_DIR = ./executor/
REDIR_DIR = ./redirection/

PARSE_SRCS = cmd.c cmd2.c cmd3.c cmd4.c heredoc.c heredoc_utils.c parse.c parse2.c token.c token2.c
BUILTIN_SRCS = ft_pwd.c ft_env.c ft_echo.c ft_export1.c ft_export2.c ft_unset.c ft_unset2_single.c ft_cd.c ft_cd2_utils.c ft_cd3_single.c ft_exit.c ft_utils.c
UTIL_SRCS = add_no_utils_func.c env_utils.c free.c ft_utils.c get_pwd.c signal.c echoctl.c
EXEC_SRCS = exec_builtins.c do_cmd_with_pipe_utils.c do_cmd_with_pipe.c do_cmd_without_pipe_utils.c do_cmd_without_pipe.c
REDIR_SRCS = redir_in.c remove_without_redir.c redir_out.c

SRCS = main.c $(addprefix $(PARSE_DIR), $(PARSE_SRCS)) $(addprefix $(BUILTIN_DIR), $(BUILTIN_SRCS)) $(addprefix $(UTIL_DIR), $(UTIL_SRCS)) $(addprefix $(EXEC_DIR), $(EXEC_SRCS)) $(addprefix $(REDIR_DIR), $(REDIR_SRCS))
OBJS	=	$(SRCS:.c=.o)

all	:	$(NAME)

$(NAME)	:	$(OBJS)
			@make -C $(LIBFT_DIR) all
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L/goinfre/sjo/.brew/opt/readline/lib -o $@ $(OBJS)

%.o	:	%.c
		@$(CC) $(CFLAG) -I/goinfre/sjo/.brew/opt/readline/include -c $< -o $@

clean	:
			@rm -rf $(OBJS)
			@make -C $(LIBFT_DIR) clean


fclean	:	clean
			@rm -rf $(NAME)
			@make -C $(LIBFT_DIR) fclean

re	:	fclean all

.PHONY	:	all, clean, fclean, re
