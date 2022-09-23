/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 14:47:07 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <termios.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define FALSE 0
# define TRUE 1
# define INT_MAX 2147483647
# define INT_MIN -2147483648

enum e_token_type
{
	TO_COMMON,
	TO_REDIRIN,
	TO_REDIROUT,
	TO_HEREDOC,
	TO_APPEND,
	PIPE,
	DQUOTE,
	QUOTE,
	DOLLAR
};

enum e_cmd_type
{
	COMMON,
	REDIRIN,
	REDIROUT,
	HEREDOC,
	APPEND,
	REDIRARG,
	BUILTIN,
	OPTION
};

typedef struct s_env_list	t_env_list;
typedef struct s_token_node	t_token_node;
typedef struct s_cmd_node	t_cmd_node;
typedef struct s_cmd_list	t_cmd_list;
t_env_list					g_st;

struct s_env_list
{
	char	**env_list;
	int		exit_status;
};

struct s_token_node
{
	enum e_token_type	type;
	char				*token;
	t_token_node		*prev;
	t_token_node		*next;
	int					idx;
};

struct s_cmd_node
{
	enum e_cmd_type	type;
	char			*cmd;
	t_cmd_node		*prev;
	t_cmd_node		*next;
};

struct s_cmd_list
{
	int			size;
	t_cmd_node	**cmd_head;
};

/**** parse ****/
int					parse_error(int option);
int					parse_cmd(char *line, t_cmd_list **cmd_line_list);
int					make_token_list(t_token_node **token_head, char *line);
int					make_cmd_list(t_cmd_list **cmd_line_list,
						t_token_node *token_head, char *line);
int					add_common_cmd(t_cmd_node **cmd_head,
						t_token_node **curr_token, enum e_token_type type,
						char *line);
int					add_dollar_cmd(t_cmd_node **cmd_head,
						t_token_node **curr_token, char *line);
int					need_join(t_token_node *curr_token, char *line, int option);
char				*replace_dollar(char *str, int idx, char *line);
void				make_new_str(char **new_str,
						t_token_node **curr, char *line);
int					check_cmd(t_cmd_node **cmd_head, int size);
int					add_quote_cmd(t_cmd_node **cmd_head,
						t_token_node **curr_token, enum e_token_type type,
						char *line);
int					add_cmd(t_cmd_node **cmd_head, t_cmd_node *new_node);
void				tolower_str(char *str);
char				*get_lower_str(char *str);
int					add_token(t_token_node **token_head,
						t_token_node **new_node, enum e_token_type type,
						int idx);
enum e_token_type	get_token_type(char *line, int idx);
void				dquote_dollar(char **new_str, char *key);
void				join_quote(t_cmd_node **cmd_head,
						t_token_node **curr_token, char *line);
int					new_quote(t_cmd_node **cmd_head,
						t_token_node **curr_token, char *line);
void				join_dquote(t_cmd_node **cmd_head,
						t_token_node **curr, char *line);
int					new_dquote(t_cmd_node **cmd_head,
						t_token_node **curr, char *line);
int					write_heredoc(int fd, char *line);
void				cmd_check(t_cmd_list *cmd_line_list,
						t_token_node *token_head);
int					cut_cmd_line(t_cmd_node **curr_cmd_head,
						t_token_node **curr_token, char *line);
int					is_empty_line(char *line);
void				free_token_line(t_token_node *head, char *line);
void				join_cmd(t_cmd_node **cmd_head, char *token);
int					heredoc_parent(pid_t pid);

/**** utils ****/
char				**copy_envs(char **envp);
void				free_split(char **tmp);
void				free_split_part(char **tmp, int idx);
void				free_array(int **tmp, int idx);
void				free_cmd(t_cmd_list *cmd_list);
int					ft_strcmp(char *s1, char *s2);
void				set_main_signal(void);
void				set_heredoc_signal(void);
void				sigint_handler(int sig);
void				sigquit_handler(int sig);
void				sig_heredoc_handler(int sig);
char				*char_to_string(char c);
int					is_white_space(char c);
void				ft_free(char **s);
void				echoctl_off(void);
void				echoctl_on(void);
int					is_not_common(enum e_cmd_type type);

/**** redirection ****/
int					mini_heredoc(t_cmd_node **curr_cmd);
void				redir_in(t_cmd_node *node);
char				*have_redir_in(t_cmd_node *node);
void				redir_out(t_cmd_node *node);
t_cmd_node			*have_redir_out(t_cmd_node *node);
t_cmd_node			*remove_redir(t_cmd_node *head);
void				remove_temp_file(void);
void				move_heredoc_curser(int fd);
int					heredoc_child(char *delimiter);
void				make_new_dollar_string(int *idx,
						t_token_node **curr, char **new_str);

/**** executor ****/
void				exec_builtins(t_cmd_node *node);
void				exec_single_builtins(t_cmd_node *node);
char				*is_valid_cmd(t_cmd_node *node);
char				**string_array(t_cmd_node *node);
int					malloc_fd(int size, int ***fd);
int					malloc_pid(int size, pid_t **pid);
int					malloc_status(int size, int **status);
void				malloc_variables(int size,
						int ***fd, pid_t **pid, int **status);
void				exec_with_pipe(t_cmd_list *list);
void				exec_cmd(t_cmd_list *cmd_line_list);
void				execve_error(char *strerror, t_cmd_node *cmd_list);
void				free_variables(int size,
						int ***fd, pid_t **pid, int **status);

/**** builtins ****/
char				*get_value(char *key);
void				edit_env_list(char *str);
char				*get_pwd(void);
void				home_dir(char *str);
void				old_dir(void);
void				ft_cd(t_cmd_node *head);
void				ft_echo(t_cmd_node *head);
void				ft_env(void);
void				ft_exit_single_cmd(t_cmd_node *head);
void				ft_exit(t_cmd_node *head);
int					is_in_env_list(char *str);
int					is_in_envp(char *str);
void				modify_envp(char *str, int i);
char				**new_export(char *str);
int					is_valid_form(char *str);
void				ft_export(t_cmd_node *head);
void				ft_pwd(void);
int					envp_cnt(void);
int					is_valid_env(char *str);
int					is_str_in_envp(char *str);
void				ft_unset(t_cmd_node *head);
void				ft_export_single_cmd(t_cmd_node *head);
void				ft_unset_single_cmd(t_cmd_node *head);
void				ft_cd_single_cmd(t_cmd_node *head);
void				export_no_arg(t_cmd_node *head);
int					have_equal(char *str);
void				change_dir(char *str);
void				export_str(char *str);

#endif