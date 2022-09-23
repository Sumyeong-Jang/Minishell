/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:36:51 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:36:52 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_builtins(t_cmd_node	*node);
void	exec_single_builtins(t_cmd_node	*node);

void	exec_builtins(t_cmd_node	*node)
{
	if (ft_strcmp(node->cmd, "pwd") == 0)
		ft_pwd();
	if (ft_strcmp(node->cmd, "env") == 0)
		ft_env();
	if (ft_strcmp(node->cmd, "echo") == 0)
		ft_echo(node);
	if (ft_strcmp(node->cmd, "export") == 0)
		ft_export(node);
	if (ft_strcmp(node->cmd, "unset") == 0)
		ft_unset(node);
	if (ft_strcmp(node->cmd, "cd") == 0)
		ft_cd(node);
	if (ft_strcmp(node->cmd, "exit") == 0)
		ft_exit(node);
	exit(0);
}

void	exec_single_builtins(t_cmd_node	*node)
{
	if (ft_strcmp(node->cmd, "pwd") == 0)
		ft_pwd();
	if (ft_strcmp(node->cmd, "env") == 0)
		ft_env();
	if (ft_strcmp(node->cmd, "echo") == 0)
		ft_echo(node);
	if (ft_strcmp(node->cmd, "export") == 0)
		ft_export_single_cmd(node);
	if (ft_strcmp(node->cmd, "unset") == 0)
		ft_unset_single_cmd(node);
	if (ft_strcmp(node->cmd, "cd") == 0)
		ft_cd_single_cmd(node);
	if (ft_strcmp(node->cmd, "exit") == 0)
		ft_exit_single_cmd(node);
}
