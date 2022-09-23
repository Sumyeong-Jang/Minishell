/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 02:21:22 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 13:07:19 by sjo              ###   ########.fr       */
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
    // ft_memset(this_node, 0, sizeof(t_cmd_node));
    ft_memset(this_node, 0, sizeof(t_token_node));
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
    // ft_memset(this_node, 0, sizeof(t_cmd_node));
    ft_memset(this_node, 0, sizeof(t_token_node));
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