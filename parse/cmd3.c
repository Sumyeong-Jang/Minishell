/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 17:20:21 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*replace_dollar(char *str, int idx, char *line)
{
	char	**split;
	char	*ret;
	char	*name;
	int		envp_idx;

	if (idx < 1 || line[idx - 1] != '$')
		return (ft_strdup("$"));
	if (ft_strcmp(str, "\"") == 0)
		return (ft_strdup("$"));
	if (ft_strcmp(str, "?") == 0)
		return (ft_itoa(g_st.exit_status));
	envp_idx = is_in_envp(str);
	if (envp_idx == -1)
		return (ft_strdup(""));
	split = ft_split(g_st.env_list[envp_idx], '=');
	if (split == 0)
		return (0);
	ret = ft_strdup(split[1]);
	free_split(split);
	return (ret);
}

int	add_dollar_cmd(t_cmd_node **cmd_head, t_token_node **curr, char *line)
{
	t_cmd_node	*new_cmd;
	char		*str;

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

int	add_common_cmd(t_cmd_node **cmd_head, t_token_node **curr_token,
		enum e_token_type type, char *line)
{
	t_cmd_node	*new_cmd;

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

int	cut_cmd_line(t_cmd_node **curr_cmd_head,
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

void	cmd_check(t_cmd_list *cmd_line_list, t_token_node *token_head)
{
	t_token_node		*this_node;
	enum e_token_type	type;

	this_node = token_head;
	cmd_line_list->size = 1;
	while (this_node != NULL)
	{
		if (this_node->type == PIPE)
			cmd_line_list->size++;
		this_node = this_node->next;
	}
}
