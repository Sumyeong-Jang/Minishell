/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 02:24:14 by sjo               #+#    #+#             */
/*   Updated: 2022/09/21 05:55:20 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int add_cmd(t_cmd_node **cmd_head, t_cmd_node *new_node)
{
    t_cmd_node *last_node;

    if (*cmd_head == 0)
        *cmd_head = new_node;
    else
    {
        last_node = *cmd_head;
        while (last_node->next != NULL)
            last_node = last_node->next;
        last_node->next = new_node;
        new_node->prev = last_node;
    }
    return (TRUE);
}

void make_new_dollar_string(int
                                *idx,
                            t_token_node **curr, char **new_str)
{
    char *tmp_str;
    int tmp_idx;

    tmp_idx = ++(*idx);
    while (((*curr)->token[*idx] != '\0') && ((*curr)->token[*idx] != '$') &&
           (is_white_space((*curr)->token[*idx]) == 0))
        (*idx)++;
    tmp_str = ft_substr((*curr)->token, tmp_idx, *idx - tmp_idx);
    if (ft_strcmp(tmp_str, "") != 0)
        dquote_dollar(new_str, tmp_str);
    else
        *new_str = ft_strjoin(*new_str, ft_strdup("$"));
    free(tmp_str);
}

void dquote_dollar(char **new_str, char *key)
{
    int envp_idx;
    char **split;
    char *ret;

    if (ft_strcmp(key, "?") == 0)
    {
        *new_str = ft_strjoin(*new_str, ft_itoa(g_env_list.exit_status));
        return;
    }
    envp_idx = is_in_envp(key);
    if (envp_idx == -1)
        return;
    split = ft_split(g_env_list.env_list[envp_idx], '=');
    if (split == 0)
        exit(1);
    ret = ft_strdup(split[1]);
    free_split(split);
    *new_str = ft_strjoin(*new_str, ret);
}

void join_cmd(t_cmd_node **cmd_head, char *token)
{
    t_cmd_node *last_cmd;

    last_cmd = (*cmd_head);
    while (last_cmd->next != NULL)
        last_cmd = last_cmd->next;
    last_cmd->cmd = ft_strjoin(last_cmd->cmd, ft_strdup(token));
}

int add_quote_cmd(t_cmd_node **cmd_head, t_token_node **curr_token,
                  enum e_token_type type, char *line)
{
    if (type == QUOTE)
    {
        if (need_join(*curr_token, line, 1) == 1)
            join_quote(cmd_head, curr_token, line);
        else
        {
            if (new_quote(cmd_head, curr_token, line) == 0)
                return (0);
        }
    }
    else
    {
        if (need_join(*curr_token, line, 1) == 1)
            join_dquote(cmd_head, curr_token, line);
        else if (new_dquote(cmd_head, curr_token, line) == 0)
            return (0);
    }
    return (1);
}

char *replace_dollar(char *str, int idx, char *line)
{
    char **split;
    char *ret;
    char *name;
    int envp_idx;

    if (idx < 1 || line[idx - 1] != '$')
        return (ft_strdup("$"));
    if (ft_strcmp(str, "\"") == 0)
        return (ft_strdup("$"));
    if (ft_strcmp(str, "?") == 0)
        return (ft_itoa(g_env_list.exit_status));
    envp_idx = is_in_envp(str);
    if (envp_idx == -1)
        return (ft_strdup(""));
    split = ft_split(g_env_list.env_list[envp_idx], '=');
    if (split == 0)
        return (0);
    ret = ft_strdup(split[1]);
    free_split(split);
    return (ret);
}

int add_dollar_cmd(t_cmd_node **cmd_head, t_token_node **curr, char *line)
{
    t_cmd_node *new_cmd;
    char *str;

    if ((*curr)->next == NULL)
        str = ft_strdup("$");
    else
        str = replace_dollar((*curr)->next->token, (*curr)->next->idx, line);
    if (need_join(*curr, line, 2) == 1)
    {
        join_cmd(cmd_head, str);
        free(str);
        if ((*curr)->next != NULL)
            *curr = (*curr)->next;
        return (1);
    }
    new_cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
    if (new_cmd == NULL)
        return (0);
    ft_memset(new_cmd, 0, sizeof(t_cmd_node));
    new_cmd->cmd = str;
    new_cmd->type = COMMON;
    add_cmd(cmd_head, new_cmd);
    if ((*curr)->next != NULL)
        *curr = (*curr)->next;
    return (1);
}

int add_common_cmd(t_cmd_node **cmd_head, t_token_node **curr_token,
                   enum e_token_type type, char *line)
{
    t_cmd_node *new_cmd;

    if (need_join(*curr_token, line, 3) == 1)
    {
        join_cmd(cmd_head, (*curr_token)->token);
        return (1);
    }
    new_cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
    if (new_cmd == NULL)
        return (0);
    ft_memset(new_cmd, 0, sizeof(t_cmd_node));
    new_cmd->cmd = ft_strdup((*curr_token)->token);
    new_cmd->type = (enum e_cmd_type)type;
    add_cmd(cmd_head, new_cmd);
    return (1);
}

static int cut_cmd_line(t_cmd_node **curr_cmd_head,
                        t_token_node **curr_token, char *line)
{
    while (*curr_token != NULL && (*curr_token)->type != PIPE)
    {
        if ((*curr_token)->type == DQUOTE || (*curr_token)->type == QUOTE)
        {
            if (add_quote_cmd(curr_cmd_head, curr_token,
                              (*curr_token)->type, line) == 0)
                return (0);
        }
        else if ((*curr_token)->token[0] == '$')
        {
            if (add_dollar_cmd(curr_cmd_head, curr_token, line) == 0)
                return (0);
        }
        else if (add_common_cmd(curr_cmd_head, curr_token,
                                (*curr_token)->type, line) == 0)
            return (0);
        *curr_token = (*curr_token)->next;
    }
    if ((*curr_token) != NULL)
        *curr_token = (*curr_token)->next;
    return (1);
}

static void cmd_check(t_cmd_list *cmd_line_list, t_token_node *token_head)
{
    t_token_node *this_node;
    enum e_token_type type;

    this_node = token_head;
    cmd_line_list->size = 1;
    while (this_node != NULL)
    {
        if (this_node->type == PIPE)
            cmd_line_list->size++;
        this_node = this_node->next;
    }
}

static int is_not_common(enum e_cmd_type type)
{
    if (type == REDIRIN || type == REDIROUT || type == HEREDOC || type == APPEND)
        return (1);
    else
        return (0);
}

int mini_heredoc(t_cmd_node **curr_cmd)
{
    pid_t pid;
    int status;
    int ret;

    signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid <= -1)
		exit(1);
    else if (pid == 0)
        heredoc_child((*curr_cmd)->cmd);
    else
    {
        waitpid(pid, &status, 0);
        ret = status / 256;
        set_main_signal();
        if (ret == 130 || ret == 1)
        {
            g_env_list.exit_status = 1;
            remove_temp_file();
            return (0);
        }
        (*curr_cmd)->prev->type = REDIRIN;
        free((*curr_cmd)->cmd);
        (*curr_cmd)->cmd = ft_strdup("sjo.sumjang");
    }
    return (1);
}

static int check_redir(t_cmd_node **curr_cmd)
{
    enum e_cmd_type type;

    type = (*curr_cmd)->type;
    if (type == REDIRIN || type == REDIROUT || type == HEREDOC ||
        type == APPEND)
    {
        if ((*curr_cmd)->next == NULL ||
            is_not_common((*curr_cmd)->next->type) == 1)
            return (parse_error(3));
        else
        {
            *curr_cmd = (*curr_cmd)->next;
            (*curr_cmd)->type = REDIRARG;
        }
    }
    if (type == HEREDOC)
    {
        if (mini_heredoc(curr_cmd) == 0)
            return (0);
    }
    return (1);
}

static int is_echo_option(t_cmd_node *node)
{
    int idx;

    if (node != NULL && node->cmd[0] == '-')
    {
        idx = 1;
        while (node->cmd[idx] != '\0')
        {
            if (node->cmd[idx] != 'n')
                return (0);
            idx++;
        }
        return (1);
    }
    return (0);
}

static int is_builtin(t_cmd_node **curr)
{
    int ret;
    char *temp;

    temp = get_lower_str((*curr)->cmd);
    ret = 0;
    if (ft_strcmp(temp, "echo") == 0)
    {
        (*curr)->type = BUILTIN;
        if (is_echo_option((*curr)->next) == 1)
        {
            *curr = (*curr)->next;
            (*curr)->type = OPTION;
        }
        ret = 1;
    }
    else if (!ft_strcmp(temp, "cd") || !ft_strcmp(temp, "pwd") ||
             !ft_strcmp(temp, "export") || !ft_strcmp(temp, "unset") ||
             !ft_strcmp(temp, "env") || !ft_strcmp(temp, "exit"))
    {
        (*curr)->type = BUILTIN;
        ret = 1;
    }
    free(temp);
    return (ret);
}

int check_cmd(t_cmd_node **cmd_head, int size)
{
    int idx;
    int flag;
    t_cmd_node *curr;
    char *temp;

    idx = 0;
    while (idx < size)
    {
        flag = 0;
        curr = cmd_head[idx];
        while (curr != NULL)
        {
            if (check_redir(&curr) == 0)
                return (0);
            if (flag == 0 && curr->type == COMMON &&
                (is_builtin(&curr) == 1))
                tolower_str(curr->cmd);
            if (flag == 0 && curr->type == COMMON || curr->type == BUILTIN)
                flag = 1;
            curr = curr->next;
        }
        idx++;
    }
    return (1);
}

int make_cmd_list(t_cmd_list **cmd_line_list,
                  t_token_node *token_head, char *line)
{
    int idx;
    t_token_node *curr_token;

    cmd_check(*cmd_line_list, token_head);
    (*cmd_line_list)->cmd_head = (t_cmd_node **)malloc(sizeof(t_cmd_node *) *
                                                       (*cmd_line_list)->size);
    if ((*cmd_line_list)->cmd_head == NULL)
        return (0);
    ft_memset((*cmd_line_list)->cmd_head, 0, sizeof(t_cmd_node *) * (*cmd_line_list)->size);
    idx = 0;
    curr_token = token_head;
    while (idx < (*cmd_line_list)->size)
    {
        if (cut_cmd_line(&((*cmd_line_list)->cmd_head[idx]),
                         &curr_token, line) == 0)
            return (0);
        idx++;
        if (idx < (*cmd_line_list)->size && curr_token == NULL)
            return (parse_error(4));
    }
    if (check_cmd((*cmd_line_list)->cmd_head, (*cmd_line_list)->size) == 0)
        return (0);
    return (1);
}