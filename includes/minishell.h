/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 14:47:07 by sjo               #+#    #+#             */
/*   Updated: 2022/09/18 18:52:53 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

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

typedef struct s_env_list t_env_list;
typedef struct s_token_node t_token_node;
typedef struct s_cmd_node t_cmd_node;
typedef struct s_cmd_list t_cmd_list;
t_env_list g_env_list;

struct s_env_list
{
    char **env_list;
    // int exit_status;
};

struct s_token_node
{
    enum e_token_type type;
    char *token;
    t_token_node *prev;
    t_token_node *next;
    int idx;
};

struct s_cmd_node
{
    enum e_cmd_type type;
    char *cmd;
    t_cmd_node *prev;
    t_cmd_node *next;
};

struct s_cmd_list
{
    int size;
    t_cmd_node **cmd_head;
};

#endif