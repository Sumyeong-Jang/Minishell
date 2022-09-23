/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 17:19:28 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_cmd(t_cmd_node **cmd_head, t_cmd_node *new_node)
{
	t_cmd_node	*last_node;

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

void	make_new_dollar_string(int *idx, t_token_node **curr, char **new_str)
{
	char	*tmp_str;
	int		tmp_idx;

	tmp_idx = ++(*idx);
	while (((*curr)->token[*idx] != '\0') && ((*curr)->token[*idx] != '$')
		&& (is_white_space((*curr)->token[*idx]) == 0))
		(*idx)++;
	tmp_str = ft_substr((*curr)->token, tmp_idx, *idx - tmp_idx);
	if (ft_strcmp(tmp_str, "") != 0)
		dquote_dollar(new_str, tmp_str);
	else
		*new_str = ft_strjoin(*new_str, ft_strdup("$"));
	free(tmp_str);
}

void	dquote_dollar(char **new_str, char *key)
{
	int		envp_idx;
	char	**split;
	char	*ret;

	if (ft_strcmp(key, "?") == 0)
	{
		*new_str = ft_strjoin(*new_str, ft_itoa(g_st.exit_status));
		return ;
	}
	envp_idx = is_in_envp(key);
	if (envp_idx == -1)
		return ;
	split = ft_split(g_st.env_list[envp_idx], '=');
	if (split == 0)
		exit(1);
	ret = ft_strdup(split[1]);
	free_split(split);
	*new_str = ft_strjoin(*new_str, ret);
}

void	join_cmd(t_cmd_node **cmd_head, char *token)
{
	t_cmd_node	*last_cmd;

	last_cmd = (*cmd_head);
	while (last_cmd->next != NULL)
		last_cmd = last_cmd->next;
	last_cmd->cmd = ft_strjoin(last_cmd->cmd, ft_strdup(token));
}

int	add_quote_cmd(t_cmd_node **cmd_head, t_token_node **curr_token,
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
