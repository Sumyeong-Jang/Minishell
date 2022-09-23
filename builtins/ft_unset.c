/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:16:00 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:16:01 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(t_cmd_node *head);
int		ft_unset_2(t_cmd_node *head, int flag);
int		unset_error(char *cmd);

void	ft_unset(t_cmd_node *head)
{
	int			flag;

	flag = ft_unset_2(head, FALSE);
	if (flag == TRUE)
		exit(1);
}

int	ft_unset_2(t_cmd_node *head, int flag)
{
	t_cmd_node	*curr_node;
	int			loc;

	curr_node = head->next;
	while (curr_node != NULL)
	{
		if (is_valid_form(curr_node->cmd) == FALSE)
			flag = unset_error(curr_node->cmd);
		curr_node = curr_node->next;
	}
	return (flag);
}

int	unset_error(char *cmd)
{
	ft_putstr_fd("bash : unset : ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	return (TRUE);
}
