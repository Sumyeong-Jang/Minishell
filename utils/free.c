/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 15:10:18 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 14:43:59 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free(char **s);
void	free_array(int **tmp, int idx);

void	ft_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		s[i] = NULL;
		i++;
	}
	free(s);
	s = NULL;
}

void	free_array(int **tmp, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		free(tmp[i]);
		tmp[i] = NULL;
		i++;
	}
	free(tmp);
	tmp = NULL;
}

void	free_cmd(t_cmd_list *cmd_list)
{
	t_cmd_node	*curr;
	t_cmd_node	*temp;
	int			i;

	i = 0;
	while (i < cmd_list->size)
	{
		if (cmd_list->cmd_head == NULL)
			break ;
		curr = cmd_list->cmd_head[i];
		while (curr != NULL)
		{
			temp = curr;
			if (curr->cmd != 0)
				free(curr->cmd);
			curr = curr->next;
			free(temp);
		}
		i++;
	}
	free(cmd_list->cmd_head);
	free(cmd_list);
	cmd_list = 0;
}

void	free_split(char **tmp)
{
	int	i;
	int	cnt;

	i = -1;
	cnt = 0;
	while (tmp[++i])
		cnt++;
	i = -1;
	while (++i < cnt)
		free(tmp[i]);
	free(tmp);
	tmp = NULL;
}

void	free_split_part(char **tmp, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		free(tmp[i]);
		tmp[i] = NULL;
		i++;
	}
	free(tmp);
	tmp = NULL;
}
