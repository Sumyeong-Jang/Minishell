/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:46 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_export(t_cmd_node *head);
void	ft_export_single_cmd(t_cmd_node *head);
void	export_no_arg(t_cmd_node *head);
int		is_valid_form(char *str);

void	ft_export(t_cmd_node *head)
{
	int			exit_code;
	t_cmd_node	*curr_node;

	curr_node = head->next;
	exit_code = FALSE;
	export_no_arg(head);
	while (curr_node != NULL)
	{
		if (is_valid_form(curr_node->cmd) == FALSE)
		{
			exit_code = TRUE;
			ft_putstr_fd("bash : export : ", STDERR_FILENO);
			ft_putstr_fd(curr_node->cmd, STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		}
		curr_node = curr_node->next;
	}
	if (exit_code == TRUE)
		exit(exit_code);
}

void	ft_export_single_cmd(t_cmd_node *head)
{
	int			idx;
	t_cmd_node	*curr_node;

	curr_node = head->next;
	export_no_arg(head);
	while (curr_node != NULL)
	{
		if (is_valid_form(curr_node->cmd) == FALSE)
		{
			ft_putstr_fd("bash : export : ", STDERR_FILENO);
			ft_putstr_fd(curr_node->cmd, STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			g_st.exit_status = 1;
		}
		else if ((have_equal(curr_node->cmd) == TRUE))
		{
			idx = is_in_envp(curr_node->cmd);
			if (idx != -1)
				modify_envp(curr_node->cmd, idx);
			else
				g_st.env_list = new_export(curr_node->cmd);
		}
		curr_node = curr_node->next;
	}
}

void	export_no_arg(t_cmd_node *head)
{
	int	i;

	i = 0;
	if (head->next == NULL)
	{
		while (g_st.env_list[i])
		{
			printf("declare -x %s\n", g_st.env_list[i]);
			i++;
		}
	}
}
