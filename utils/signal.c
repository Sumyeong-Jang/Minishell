/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 19:06:28 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_heredoc_handler(int sig)
{
	(void)sig;
	ft_putendl_fd("", STDOUT_FILENO);
	exit(130);
}

void	set_heredoc_signal(void)
{
	signal(SIGINT, sig_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_handler(int sig)
{
	pid_t	pid;

	pid = waitpid(-1, NULL, WNOHANG);
	if (sig == SIGINT)
	{
		if (pid == -1)
		{
			ft_putendl_fd("", STDOUT_FILENO);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			g_st.exit_status = 1;
		}
		else
		{
			g_st.exit_status = 130;
			ft_putendl_fd("", STDOUT_FILENO);
		}
	}
}

void	sigquit_handler(int sig)
{
	pid_t	pid;

	pid = waitpid(-1, NULL, WNOHANG);
	if (sig == SIGQUIT)
	{
		if (pid == -1)
		{
			rl_on_new_line();
			rl_redisplay();
		}
		else
		{
			g_st.exit_status = 131;
			ft_putstr_fd("Quit: ", STDOUT_FILENO);
			ft_putnbr_fd(sig, STDOUT_FILENO);
			ft_putendl_fd("", STDOUT_FILENO);
		}
	}
}

void	set_main_signal(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
