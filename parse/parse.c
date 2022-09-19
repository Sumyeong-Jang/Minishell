/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 17:37:44 by sjo               #+#    #+#             */
/*   Updated: 2022/09/19 15:59:56 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int add_token(t_token_node **token_head, t_token_node **new_node,
              enum e_token_type type, int idx)
{
    t_token_node *last_node;

    (*new_node)->type = type;
    (*new_node)->idx = idx;
    if (*token_head == NULL)
        *token_head = *new_node;
    else
    {
        last_node = *token_head;
        while (last_node->next != NULL)
            last_node = last_node->next;
        last_node->next = *new_node;
        (*new_node)->prev = last_node;
    }
    return (1);
}

enum e_token_type get_token_type(char *line, int idx)
{
    if (line[idx] == '"')
        return (DQUOTE);
    else if (line[idx] == '\'')
        return (QUOTE);
    else if (line[idx] == '|')
        return (PIPE);
    else if (line[idx] == '$')
        return (DOLLAR);
    else if (line[idx] == '<')
    {
        if (line[idx + 1] != '\0' && line[idx + 1] == '<')
            return (TO_HEREDOC);
        return (TO_REDIRIN);
    }
    else if (line[idx] == '>')
    {
        if (line[idx + 1] != '\0' && line[idx + 1] == '>')
            return (TO_APPEND);
        return (TO_REDIROUT);
    }
    return (TO_COMMON);
}

static int add_quote_token(t_token_node **this_node,
                           int *idx, enum e_token_type type, char *line)
{
    char quote;
    int start;

    quote = line[*idx];
    (*this_node)->type = type;
    (*idx)++;
    start = *idx;
    while (line[*idx] != '\0' && line[*idx] != quote)
        (*idx)++;
    if (line[*idx] == '\0')
        return (parse_error(1));
    (*this_node)->token = ft_substr(line, start, *idx - start);
    return (1);
}

static void add_spacial_token2(t_token_node **this_node, enum e_token_type type)
{
    if (type == TO_REDIRIN)
        (*this_node)->token = ft_strdup("<");
    else if (type == TO_REDIROUT)
        (*this_node)->token = ft_strdup(">");
    else if (type == TO_HEREDOC)
        (*this_node)->token = ft_strdup("<<");
    else if (type == TO_APPEND)
        (*this_node)->token = ft_strdup(">>");
    else if (type == PIPE)
        (*this_node)->token = ft_strdup("|");
}

static int add_spacial_token(t_token_node **token_head,
                             enum e_token_type type, int *idx, char *line)
{
    t_token_node *this_node;
    int temp;

    this_node = (t_token_node *)malloc(sizeof(t_token_node));
    if (this_node == NULL)
        return (0);
    ft_memset(this_node, 0, sizeof(t_cmd_node));
    add_spacial_token2(&this_node, type);
    if (type == DQUOTE || type == QUOTE)
    {
        temp = *idx;
        if (add_quote_token(&this_node, idx, type, line) == 0)
        {
            free(this_node);
            this_node = 0;
            return (0);
        }
        return (add_token(token_head, &this_node, type, temp));
    }
    else if (type == DOLLAR)
        this_node->token = ft_strdup("$");
    if (this_node->token == NULL)
        return (0);
    return (add_token(token_head, &this_node, type, *idx));
}

static int add_common_token(t_token_node **token_head, char *line, int *idx)
{
    t_token_node *this_node;
    int start;

    this_node = (t_token_node *)malloc(sizeof(t_token_node));
    if (this_node == NULL)
        return (0);
    ft_memset(this_node, 0, sizeof(t_cmd_node));
    start = *idx;
    while (line[*idx] != '\0' && !(line[*idx] == ' ' || line[*idx] >= 9 && line[*idx] <= 13) &&
           (get_token_type(line, *idx) == TO_COMMON))
        (*idx)++;
    this_node->token = ft_substr(line, start, (*idx - start));
    if (this_node->token == NULL)
        return (0);
    return (add_token(token_head, &this_node, TO_COMMON, start));
}

int make_token_list(t_token_node **token_head, char *line)
{
    int i;
    enum e_token_type type;

    i = 0;
    while (line[i] != '\0')
    {
        while (line[i] != '\0' && (line[i] == ' ' ||
                                   line[i] >= 9 && line[i] <= 13))
            i++;
        if (line[i] == '\0')
            return (1);
        type = get_token_type(line, i);
        if (type == TO_APPEND || type == TO_HEREDOC)
            i++;
        if (type != TO_COMMON)
        {
            if (add_spacial_token(token_head, type, &i, line) == 0)
                return (0);
            i++;
        }
        else if (add_common_token(token_head, line, &i) == 0)
            return (0);
    }
    return (1);
}

// 왜 static인가,,,,
static int is_empty_line(char *line)
{
    int i;

    i = 0;
    while (line[i] != '\0')
    {
        if (line[i] != '\0' && !(line[i] == ' ' ||
                                 line[i] >= 9 && line[i] <= 13))
            return (0);
        i++;
    }
    return (1);
}

int need_join(t_token_node *curr_token, char *line, int option)
{
    char c;

    if (curr_token->idx - 1 > 0)
        c = line[curr_token->idx - 1];
    else
        c = '\0';
    if (option == 1)
    {
        if (c != '\0' && !(c == ' ' || c >= 9 && c <= 13))
            return (1);
        return (0);
    }
    else if (option == 2)
    {
        if (c != '\0' && (c == '\'' || c == '"' ||
                          !(c == ' ' || c >= 9 && c <= 13)))
            return (1);
        return (0);
    }
    else
    {
        if (c != '\0' && (c == '\'' || c == '"'))
            return (1);
        return (0);
    }
}

static void join_quote(t_cmd_node **cmd_head,
                       t_token_node **curr_token, char *line)
{
    int start;
    int end;
    char *new_str;
    t_cmd_node *last_cmd;

    new_str = ft_strdup((*curr_token)->token);
    last_cmd = (*cmd_head);
    while (last_cmd->next != NULL)
        last_cmd = last_cmd->next;
    last_cmd->cmd = ft_strjoin(last_cmd->cmd, new_str);
}

static int new_quote(t_cmd_node **cmd_head,
                     t_token_node **curr_token, char *line)
{
    int start;
    int end;
    char *new_str;
    t_cmd_node *new_cmd;

    new_cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
    if (new_cmd == NULL)
        return (0);
    ft_memset(new_cmd, 0, sizeof(t_cmd_node));
    new_str = ft_strdup((*curr_token)->token);
    new_cmd->cmd = new_str;
    new_cmd->type = COMMON;
    add_cmd(cmd_head, new_cmd);
    return (1);
}

static void join_dquote(t_cmd_node **cmd_head,
                        t_token_node **curr, char *line)
{
    char *new_str;
    t_cmd_node *last_cmd;

    new_str = ft_strdup("");
    make_new_str(&new_str, curr, line);
    last_cmd = (*cmd_head);
    while (last_cmd->next != NULL)
        last_cmd = last_cmd->next;
    last_cmd->cmd = ft_strjoin(last_cmd->cmd, new_str);
}

static int new_dquote(t_cmd_node **cmd_head,
                      t_token_node **curr, char *line)
{
    char *new_str;
    t_cmd_node *new_cmd;

    new_str = ft_strdup("");
    make_new_str(&new_str, curr, line);
    new_cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
    if (new_cmd == NULL)
    {
        free(new_str);
        return (0);
    }
    ft_memset(new_cmd, 0, sizeof(t_cmd_node));
    new_cmd->cmd = new_str;
    new_cmd->type = COMMON;
    add_cmd(cmd_head, new_cmd);
    return (1);
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

static int cmd_check(t_cmd_list *cmd_line_list, t_token_node *token_head)
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
    return (1);
}

int parse_error(int option)
{
    g_env_list.exit_status = 1;
    if (option == 1)
    {
        g_env_list.exit_status = 258;
        printf("ERROR : ’(\") must be paired\n");
    }
    else if (option == 2)
        printf("ERROR : Memory allocation failed\n");
    else if (option == 3)
    {
        g_env_list.exit_status = 258;
        printf("syntax error near unexpected token\n");
    }
    else if (option == 4)
    {
        printf("ERROR : Invaild pipe\n");
    }
    else if (option == 5)
    {
        printf("ERROR : Failed to open file\n");
    }
    return (0);
}

int make_cmd_list(t_cmd_list **cmd_line_list,
                  t_token_node *token_head, char *line)
{
    int idx;
    t_token_node *curr_token;

    if (cmd_check(*cmd_line_list, token_head) == 0)
        return (0);
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

int parse_cmd(char *line, t_cmd_list **cmd_line_list)
{
    t_token_node *token_head;

    token_head = NULL;
    // ft_memset(*cmd_line_list, 0, sizeof(t_cmd_list));
    // read_cmd_line(&line);
    if (is_empty_line(line) == 1)
    {
        free_token_line(token_head, line);
        return (0);
    }
    add_history(line);
    if (make_token_list(&token_head, line) == 0)
    {
        free_token_line(token_head, line);
        return (0);
    }
    if (make_cmd_list(cmd_line_list, token_head, line) == 0)
    {
        free_token_line(token_head, line);
        return (0);
    }
    free_token_line(token_head, line);
    return (1);
}