/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 17:37:44 by sjo               #+#    #+#             */
/*   Updated: 2022/09/18 19:05:39 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int parse_cmd(t_cmd_list **cmd_line_list)
{
    char *line;
    t_token_node *token_head;

    token_head = NULL;
    // ft_memset(*cmd_line_list, 0, sizeof(t_cmd_list));
    read_cmd_line(&line);
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