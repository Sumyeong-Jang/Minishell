/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 17:24:00 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 17:24:12 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_token(t_token_node **token_head, t_token_node **new_node,
		enum e_token_type type, int idx)
{
	t_token_node	*last_node;

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

enum e_token_type	get_token_type(char *line, int idx)
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
