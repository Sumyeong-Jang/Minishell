/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 17:37:44 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 17:21:55 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	new_quote(t_cmd_node **cmd_head,
		t_token_node **curr_token, char *line)
{
	int			start;
	int			end;
	char		*new_str;
	t_cmd_node	*new_cmd;

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

void	join_dquote(t_cmd_node **cmd_head,
			t_token_node **curr, char *line)
{
	char		*new_str;
	t_cmd_node	*last_cmd;

	new_str = ft_strdup("");
	make_new_str(&new_str, curr, line);
	last_cmd = (*cmd_head);
	while (last_cmd->next != NULL)
		last_cmd = last_cmd->next;
	last_cmd->cmd = ft_strjoin(last_cmd->cmd, new_str);
}

void	make_new_str(char **new_str, t_token_node **curr, char *line)
{
	int	idx;
	int	len;

	idx = 0;
	len = ft_strlen((*curr)->token);
	if (len == 0)
		return ;
	while (idx < len)
	{
		if ((*curr)->token[idx] == '$')
			make_new_dollar_string(&idx, curr, new_str);
		else
		{
			*new_str = ft_strjoin(*new_str,
					char_to_string((*curr)->token[idx]));
			idx++;
		}
	}
}

int	new_dquote(t_cmd_node **cmd_head,
		t_token_node **curr, char *line)
{
	char		*new_str;
	t_cmd_node	*new_cmd;

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

int	parse_cmd(char *line, t_cmd_list **cmd_line_list)
{
	t_token_node	*token_head;

	token_head = NULL;
	if (is_empty_line(line) == 1)
	{
		free_token_line(token_head, line);
		return (0);
	}
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
