/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_out.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:27:56 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:27:56 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		redir_out(t_cmd_node *node);
t_cmd_node	*have_redir_out(t_cmd_node *node);
void		do_redir_out(char *cmd);
void		do_redir_append(char *cmd);

void	redir_out(t_cmd_node *node)
{
	t_cmd_node	*outfile;
	int			out_fd;

	outfile = have_redir_out(node);
	if (outfile != NULL)
	{
		if (outfile->type == REDIROUT)
			out_fd = open(outfile->next->cmd, O_RDWR, 0644);
		else
			out_fd = open(outfile->next->cmd, O_RDWR | O_APPEND, 0644);
		if (out_fd < 0)
		{
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			exit(1);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

t_cmd_node	*have_redir_out(t_cmd_node *node)
{
	t_cmd_node	*curr;
	t_cmd_node	*last_redirout;

	curr = node;
	last_redirout = NULL;
	while (curr != NULL)
	{
		if (curr->type == REDIROUT)
		{
			do_redir_out(curr->next->cmd);
			last_redirout = curr;
		}
		else if (curr->type == APPEND)
		{
			do_redir_append(curr->next->cmd);
			last_redirout = curr;
		}
		curr = curr->next;
	}
	return (last_redirout);
}

void	do_redir_out(char *cmd)
{
	int			fd;
	struct stat	s;

	if (stat(cmd, &s) == 0)
	{
		fd = open(cmd, O_RDWR | O_TRUNC, 0644);
		if (fd < 0)
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		close(fd);
	}
	else
	{
		fd = open(cmd, O_RDWR | O_CREAT, 0644);
		if (fd < 0)
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		close(fd);
	}
}

void	do_redir_append(char *cmd)
{
	int			fd;
	struct stat	s;

	if (stat(cmd, &s) != 0)
	{
		fd = open(cmd, O_RDWR | O_CREAT, 0644);
		if (fd < 0)
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
		close(fd);
	}
}
