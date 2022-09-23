/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 17:37:44 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 14:25:43 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_empty_line(char *line)
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

int	parse_error(int option)
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

static void	free_token_line(t_token_node *head, char *line)
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
