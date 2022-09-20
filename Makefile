CC = cc
NAME = minishell
CFLAGS	=	-Wall -Wextra -Werror -fsanitize=address
LIBFT_DIR = ./libft/
PARSE_DIR = ./parse/
BUILTIN_DIR = ./builtins/
UTIL_DIR = ./util/
EXEC_DIR = ./exec/
REDIR_DIR = ./redirection/

PARSE_SRCS = cmd.c heredoc.c parse.c token.c
BUILTIN_SRCS = ft_pwd.c ft_env.c ft_echo.c ft_export.c ft_unset.c ft_cd.c ft_cd_util.c ft_exit.c ft_export_util.c ft_cd_single.c ft_unset_single.c
UTIL_SRCS = ft_strcmp.c copy_env.c tolowerstr.c signal_set.c signal_handler.c free.c parse_utils.c
EXEC_SRCS = exec_bulitin.c exec_cmd_util.c exec_cmd_with_pipe.c exec_cmd_without_pipe.c exec_cmd_with_pipe_util.c
REDIR_SRCS = redir_in.c without_redir.c redir_out.c heredoc.c heredoc_utils.c

SRCS = main.c $(addprefix $(PARSE_DIR), $(PARSE_SRCS)) $(addprefix $(BUILTIN_DIR), $(BUILTIN_SRCS)) $(addprefix $(UTIL_DIR), $(UTIL_SRCS)) $(addprefix $(EXEC_DIR), $(EXEC_SRCS)) $(addprefix $(REDIR_DIR), $(REDIR_SRCS))
OBJS	=	$(SRCS:.c=.o)

all	:	$(NAME)

$(NAME)	:	$(OBJS)
			@make -C $(LIBFT_DIR) all
			@$(CC) $(CFLAGS) -L$(LIBFT_DIR) -lft -lreadline -L${HOME}/.brew/opt/readline/lib -o $@ $(OBJS)
			@printf "✅ \033[0;32m$(NAME) was created.\033[0m\n"

%.o	:	%.c
		@$(CC) $(CFLAG) -I${HOME}/.brew/opt/readline/include -c $< -o $@

clean	:
			@rm -rf $(OBJS)
			@make -C $(LIBFT_DIR) clean
			@printf "🚮 $(NAME)'s object files were removed.\n"


fclean	:	clean
			@rm -rf $(NAME)
			@make -C $(LIBFT_DIR) fclean
			@printf "🚮 $(NAME) was removed.\n"

re	:	fclean all

.PHONY	:	all, clean, fclean, re