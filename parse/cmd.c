/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 02:24:14 by sjo               #+#    #+#             */
/*   Updated: 2022/09/21 03:26:11 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int check_cmd(t_cmd_node **cmd_heads, int size)
{
    int idx;
    int flag;
    t_cmd_node *curr;
    char *temp;

    idx = 0;
    while (idx < size)
    {
        flag = 0;
        curr = cmd_heads[idx];
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