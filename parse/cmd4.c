/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 17:22:41 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_not_common(enum e_cmd_type type)
{
	if (type == REDIRIN || type == REDIROUT || type == HEREDOC
		|| type == APPEND)
		return (1);
	else
		return (0);
}

int	heredoc_parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	set_main_signal();
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_st.exit_status = 1;
			remove_temp_file();
			return (0);
		}
	}
	return (1);
}

int	mini_heredoc(t_cmd_node **curr_cmd)
{
	pid_t	pid;
	int		status;

	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid <= -1)
		exit(1);
	else if (pid == 0)
		heredoc_child((*curr_cmd)->cmd);
	else
	{
		if (heredoc_parent(pid) == 0)
			return (0);
		(*curr_cmd)->prev->type = REDIRIN;
		free((*curr_cmd)->cmd);
		(*curr_cmd)->cmd = ft_strdup("sjo.sumjang");
	}
	return (1);
}
