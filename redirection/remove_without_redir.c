/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_without_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:27:59 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:28:00 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd_node	*remove_redir(t_cmd_node *head);
t_cmd_node	*cmd_dup(t_cmd_node *src_node);
void		free_single_cmd_list(t_cmd_node *head);

t_cmd_node	*remove_redir(t_cmd_node *head)
{
	t_cmd_node	*new_list_head;
	t_cmd_node	*new_node;
	t_cmd_node	*curr;

	new_list_head = NULL;
	curr = head;
	while (curr != NULL)
	{
		if (curr->type == COMMON || curr->type == BUILTIN || \
		curr->type == OPTION)
		{
			new_node = cmd_dup(curr);
			if (new_node == NULL || add_cmd(&new_list_head, new_node) == FALSE)
			{
				free_single_cmd_list(new_list_head);
				return (NULL);
			}
		}
		curr = curr->next;
	}
	return (new_list_head);
}

char	*without_redir(t_cmd_node *node)
{
	t_cmd_node	*curr;
	char		*ret;

	ret = NULL;
	curr = node;
	while (curr != NULL)
	{
		if (curr->type == COMMON)
		{
			ret = ft_strjoin(ret, ft_strdup(curr->cmd));
			ret = ft_strjoin(ret, ft_strdup(" "));
		}
		curr = curr->next;
	}
	return (ret);
}

void	free_single_cmd_list(t_cmd_node *head)
{
	t_cmd_node	*curr;
	t_cmd_node	*temp;

	curr = head;
	while (curr != NULL)
	{
		temp = curr;
		if (curr->cmd != NULL)
			free(curr->cmd);
		curr = curr->next;
		free(temp);
	}
}

t_cmd_node	*cmd_dup(t_cmd_node *src_node)
{
	t_cmd_node	*new_node;

	new_node = (t_cmd_node *)malloc(sizeof(t_cmd_node));
	if (new_node == NULL)
		exit(1);
	ft_memset(new_node, 0, sizeof(t_cmd_node));
	new_node->cmd = ft_strdup(src_node->cmd);
	if (new_node->cmd == NULL)
	{
		free(new_node);
		return (NULL);
	}
	new_node->type = src_node->type;
	return (new_node);
}
