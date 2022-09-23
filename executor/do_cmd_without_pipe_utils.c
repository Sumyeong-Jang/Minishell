/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cmd_without_pipe_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:36:40 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:36:41 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*is_valid_cmd(t_cmd_node *node);
char	*is_valid_cmd_path(t_cmd_node *node);
void	path_error(char *str, t_cmd_node *node);
char	**string_array(t_cmd_node *node);
void	execve_error(char *strerror, t_cmd_node *cmd_list);

char	*is_valid_cmd(t_cmd_node *node)
{
	struct stat	s;
	int			i;
	char		*str;

	i = -1;
	str = NULL;
	if (stat(node->cmd, &s) == 0)
	{
		str = ft_strdup(node->cmd);
		return (str);
	}
	str = is_valid_cmd_path(node);
	return (str);
}

char	*is_valid_cmd_path(t_cmd_node *node)
{
	char		**tmp;
	int			i;
	char		*str;
	char		*str2;
	struct stat	s;

	i = -1;
	str = get_value("PATH");
	path_error(str, node);
	tmp = ft_split(str, ':');
	free(str);
	while (tmp[++i])
	{
		str = ft_strjoin(ft_strdup("/"), ft_strdup(node->cmd));
		str2 = ft_strjoin(ft_strdup(tmp[i]), str);
		if (stat(str2, &s) == 0)
		{
			free_split(tmp);
			return (str2);
		}
		free(str2);
	}
	free_split(tmp);
	return (NULL);
}

void	path_error(char *str, t_cmd_node *node)
{
	if (str == NULL)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(node->cmd, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		exit(127);
	}
}

char	**string_array(t_cmd_node *node)
{
	t_cmd_node	*curr;
	char		**ret;
	int			cnt;
	int			i;

	i = 0;
	cnt = 0;
	curr = node;
	while (curr != NULL && curr->type != REDIROUT)
	{
		cnt++;
		curr = curr->next;
	}
	ret = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (!ret)
		exit (1);
	curr = node;
	while (i < cnt)
	{
		ret[i] = ft_strdup(curr->cmd);
		curr = curr->next;
		i++;
	}
	ret[cnt] = NULL;
	return (ret);
}

void	execve_error(char *strerror, t_cmd_node *cmd_list)
{
	if (ft_strcmp(strerror, "Bad address") == 0)
	{
		ft_putstr_fd("bash : ", STDERR_FILENO);
		ft_putstr_fd(cmd_list->cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	else if (ft_strcmp(strerror, "Permission denied") == 0)
	{
		ft_putstr_fd("bash : ", STDERR_FILENO);
		ft_putstr_fd(cmd_list->cmd, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		exit(126);
	}
}
