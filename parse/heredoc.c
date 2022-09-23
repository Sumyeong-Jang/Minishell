/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 03:33:47 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 14:47:43 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_temp_file(void)
{
	struct stat	s;

	if (stat("sjo.sumjang", &s) == 0)
		unlink("sjo.sumjang");
}

void	finish_heredoc(char **line, int fd, int end_status)
{
	if (*line != NULL)
		free(*line);
	if (fd >= 0)
		close(fd);
	exit(end_status);
}

int	heredoc_child(char *delimiter)
{
	int		fd;
	char	*line;

	set_heredoc_signal();
	line = NULL;
	fd = open("sjo.sumjang", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 0)
	{
		parse_error(5);
		finish_heredoc(&line, fd, 1);
	}
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			move_heredoc_curser(fd);
		if (ft_strcmp(line, delimiter) == 0)
			finish_heredoc(&line, fd, 0);
		if (write_heredoc(fd, line) == 0)
			finish_heredoc(&line, fd, 1);
		free(line);
	}
	line = NULL;
	finish_heredoc(&line, fd, 0);
}
