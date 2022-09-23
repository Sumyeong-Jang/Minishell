/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset2_single.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:16:06 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset_single_cmd(t_cmd_node *head);
int		is_valid_env(char *str);
int		envp_cnt(void);
void	unset_error_single(char *cmd);

void	ft_unset_single_cmd(t_cmd_node *head)
{
	int			loc;
	t_cmd_node	*curr_node;

	curr_node = head->next;
	while (curr_node != NULL)
	{
		if (is_valid_env(curr_node->cmd) == TRUE)
		{
			loc = is_str_in_envp(curr_node->cmd);
			if (loc != FALSE)
			{
				while (loc < envp_cnt() - 1)
				{
					free(g_st.env_list[loc]);
					g_st.env_list[loc] = ft_strdup(g_st.env_list[loc + 1]);
					loc++;
				}
				free(g_st.env_list[envp_cnt() - 1]);
				g_st.env_list[envp_cnt() - 1] = NULL;
			}
		}
		else if (is_valid_form(curr_node->cmd) == FALSE)
			unset_error_single(curr_node->cmd);
		curr_node = curr_node->next;
	}
}

int	is_valid_env(char *str)
{
	int	i;

	i = -1;
	if (ft_isalpha(str[0]) == FALSE && str[0] != '_')
		return (FALSE);
	while (str[++i])
	{
		if (ft_isalnum(str[i]) == FALSE && str[i] != '_')
			return (FALSE);
	}
	return (TRUE);
}

int	is_str_in_envp(char *str)
{
	int	i;

	i = -1;
	while (g_st.env_list[++i])
	{
		if (ft_strncmp(str, g_st.env_list[i], ft_strlen(str)) == 0)
			return (i);
	}
	return (FALSE);
}

int	envp_cnt(void)
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (g_st.env_list[++i])
		ret++;
	return (ret);
}

void	unset_error_single(char *cmd)
{
	ft_putstr_fd("bash : unset : ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	g_st.exit_status = 1;
}
