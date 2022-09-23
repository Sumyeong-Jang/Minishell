/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:30 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(t_cmd_node *head);
void	print_echo(t_cmd_node *curr, int is_option);

void	ft_echo(t_cmd_node *head)
{
	t_cmd_node	*curr_node;
	int			is_option;

	curr_node = head->next;
	if (curr_node == NULL)
	{
		printf("\n");
		g_st.exit_status = 0;
		return ;
	}
	if (curr_node->type == OPTION)
	{
		is_option = TRUE;
		curr_node = curr_node->next;
	}
	else
		is_option = FALSE;
	g_st.exit_status = 0;
	if (curr_node == NULL)
		return ;
	print_echo(curr_node, is_option);
}

void	print_echo(t_cmd_node *curr, int is_option)
{
	t_cmd_node	*curr_node;

	curr_node = curr;
	while (curr_node->next != NULL)
	{
		printf("%s ", curr_node->cmd);
		curr_node = curr_node->next;
	}
	printf("%s", curr_node->cmd);
	if (is_option == FALSE)
		printf("\n");
}
