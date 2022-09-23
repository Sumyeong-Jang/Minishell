/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 02:24:14 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 21:18:21 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redir(t_cmd_node **curr_cmd)
{
	enum e_cmd_type	type;

	type = (*curr_cmd)->type;
	if (type == REDIRIN || type == REDIROUT || type == HEREDOC
		|| type == APPEND)
	{
		if ((*curr_cmd)->next == NULL
			|| is_not_common((*curr_cmd)->next->type) == 1)
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

int	is_echo_option(t_cmd_node *node)
{
	int	idx;

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

int	is_builtin(t_cmd_node **curr)
{
	int		ret;
	char	*temp;

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
	else if (!ft_strcmp(temp, "cd") || !ft_strcmp(temp, "pwd")
		|| !ft_strcmp(temp, "export") || !ft_strcmp(temp, "unset")
		|| !ft_strcmp(temp, "env") || !ft_strcmp(temp, "exit"))
	{
		(*curr)->type = BUILTIN;
		ret = 1;
	}
	free(temp);
	return (ret);
}

int	check_cmd(t_cmd_node **cmd_head, int size)
{
	int			idx;
	int			flag;
	t_cmd_node	*curr;
	char		*temp;

	idx = 0;
	while (idx < size)
	{
		flag = 0;
		curr = cmd_head[idx];
		while (curr != NULL)
		{
			if (check_redir(&curr) == 0)
				return (0);
			if (flag == 0 && curr->type == COMMON
				&& (is_builtin(&curr) == 1))
				tolower_str(curr->cmd);
			if (flag == 0 && curr->type == COMMON || curr->type == BUILTIN)
				flag = 1;
			curr = curr->next;
		}
		idx++;
	}
	return (1);
}

int	make_cmd_list(t_cmd_list **cmd_line_list, t_token_node *token_head,
	char *line)
{
	int				idx;
	t_token_node	*curr_token;

	cmd_check(*cmd_line_list, token_head);
	(*cmd_line_list)->cmd_head = (t_cmd_node **)malloc(sizeof(t_cmd_node *)
			* (*cmd_line_list)->size);
	if ((*cmd_line_list)->cmd_head == NULL)
		return (0);
	ft_memset((*cmd_line_list)->cmd_head, 0, sizeof(t_cmd_node *)
		* (*cmd_line_list)->size);
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
