/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:41 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_exit(t_cmd_node *head);
void		ft_exit_single_cmd(t_cmd_node *head);
int			get_cnt(t_cmd_node *curr);
long long	ft_atoll(char *str);
int			get_exit_code(t_cmd_node *curr_node);

void	ft_exit(t_cmd_node *head)
{
	long long	first_argv;
	int			cnt;

	cnt = get_cnt(head);
	if (cnt > 1)
		first_argv = ft_atoll(head->next->cmd);
	if (cnt == 2 && first_argv >= INT_MIN && first_argv <= INT_MAX)
		g_st.exit_status = get_exit_code(head);
	else if (cnt > 2 && first_argv >= INT_MIN && first_argv <= INT_MAX)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		g_st.exit_status = 1;
		return ;
	}
	else if (cnt != 1)
	{
		ft_putstr_fd("bash: exit: ", STDERR_FILENO);
		ft_putstr_fd(head->next->cmd, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		g_st.exit_status = 255;
	}
	exit(g_st.exit_status);
}

void	ft_exit_single_cmd(t_cmd_node *head)
{
	long long	first_argv;
	int			cnt;

	cnt = get_cnt(head);
	if (cnt > 1)
		first_argv = ft_atoll(head->next->cmd);
	printf("exit\n");
	if (cnt == 2 && first_argv >= INT_MIN && first_argv <= INT_MAX)
		g_st.exit_status = get_exit_code(head);
	else if (cnt > 2 && first_argv >= INT_MIN && first_argv <= INT_MAX)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		g_st.exit_status = 1;
		return ;
	}
	else if (cnt != 1)
	{
		ft_putstr_fd("bash: exit: ", STDERR_FILENO);
		ft_putstr_fd(head->next->cmd, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		g_st.exit_status = 255;
	}
	exit(g_st.exit_status);
}

int	get_cnt(t_cmd_node *curr)
{
	int	cnt;

	cnt = 0;
	while (curr != NULL)
	{
		cnt++;
		curr = curr->next;
	}
	return (cnt);
}

long long	ft_atoll(char *str)
{
	long long	ret;
	int			minus;
	int			i;

	ret = 0;
	minus = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			minus = -1;
		i++;
	}
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return ((long long)INT_MAX + 1);
		ret = ret * 10 + (str[i] - '0');
		i++;
	}
	if (minus == -1)
		ret = ret * minus;
	return (ret);
}

int	get_exit_code(t_cmd_node *curr_node)
{
	int	exit_status;

	exit_status = 0;
	exit_status = ft_atoi(curr_node->next->cmd);
	if (exit_status >= 0)
		exit_status = exit_status % 256;
	else
		exit_status = 256 - ((exit_status * (-1)) % 256);
	return (exit_status);
}
