/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 17:17:19 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != '\0' && !(line[i] == ' '
				|| line[i] >= 9 && line[i] <= 13))
			return (0);
		i++;
	}
	return (1);
}

int	need_join(t_token_node *curr_token, char *line, int option)
{
	char	c;

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
		if (c != '\0' && (c == '\'' || c == '"'
				|| !(c == ' ' || c >= 9 && c <= 13)))
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

int	parse_error(int option)
{
	g_st.exit_status = 1;
	if (option == 1)
	{
		g_st.exit_status = 258;
		printf("ERROR : ’(\") must be paired\n");
	}
	else if (option == 2)
		printf("ERROR : Memory allocation failed\n");
	else if (option == 3)
	{
		g_st.exit_status = 258;
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

void	free_token_line(t_token_node *head, char *line)
{
	t_token_node	*curr;
	t_token_node	*temp;

	curr = head;
	while (curr != NULL)
	{
		temp = curr;
		free(curr->token);
		curr = curr->next;
		free(temp);
	}
	if (line != NULL)
		free(line);
}

void	join_quote(t_cmd_node **cmd_head,
			t_token_node **curr_token, char *line)
{
	int			start;
	int			end;
	char		*new_str;
	t_cmd_node	*last_cmd;

	new_str = ft_strdup((*curr_token)->token);
	last_cmd = (*cmd_head);
	while (last_cmd->next != NULL)
		last_cmd = last_cmd->next;
	last_cmd->cmd = ft_strjoin(last_cmd->cmd, new_str);
}
