/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cmd_with_pipe_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:36:09 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	malloc_variables(int size, int ***fd, pid_t **pid, int **status);
int		malloc_pid(int size, pid_t **pid);
int		malloc_fd(int size, int ***fd);
int		malloc_status(int size, int **status);
void	free_variables(int size, int ***fd, pid_t **pid, int **status);

void	malloc_variables(int size, int ***fd, pid_t **pid, int **status)
{
	if (malloc_fd(size - 1, fd) != TRUE || malloc_pid \
	(size, pid) != TRUE || malloc_status(size, status) != TRUE)
	{
		g_st.exit_status = 1;
		exit(1);
	}
}

int	malloc_pid(int size, pid_t **pid)
{
	int	i;

	i = 0;
	*pid = (pid_t *)malloc(sizeof(pid_t *) * size);
	if (*pid == NULL)
		exit (1);
	ft_memset(*pid, 0, sizeof(*pid));
	return (TRUE);
}

int	malloc_fd(int size, int ***fd)
{
	int	i;

	i = 0;
	*fd = (int **)malloc(sizeof(int *) * size);
	if (*fd == NULL)
		return (FALSE);
	ft_memset(*fd, 0, sizeof(*fd));
	while (i < size)
	{
		(*fd)[i] = (int *)malloc(sizeof(int) * 2);
		if ((*fd)[i] == NULL)
		{
			free_array((*fd), i);
			return (FALSE);
		}
		ft_memset((*fd)[i], 0, sizeof((*fd)[i]));
		i++;
	}
	return (TRUE);
}

int	malloc_status(int size, int **status)
{
	int	i;

	i = 0;
	*status = (int *)malloc(sizeof(int *) * size);
	if (*status == NULL)
		exit(1);
	ft_memset(*status, 0, sizeof(*status));
	return (TRUE);
}

void	free_variables(int size, int ***fd, pid_t **pid, int **status)
{
	free_array(*fd, size - 1);
	free(*pid);
	pid = NULL;
	free(*status);
	status = NULL;
}
