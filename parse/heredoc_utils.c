/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:45:12 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 16:56:26 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	write_dollar_heredoc(char *line, int start, int len, int fd)
{
	char	*name;
	char	*value;

	name = ft_substr(line, start, len);
	if (name == NULL)
	{
		write(fd, "\n", 1);
		return (FALSE);
	}
	value = replace_dollar(name, start, line);
	if (value == NULL)
	{
		free(name);
		write(fd, "\n", 1);
		return (FALSE);
	}
	ft_putstr_fd(value, fd);
	free(name);
	free(value);
	return (TRUE);
}

int	write_heredoc(int fd, char *line)
{
	int		idx;
	int		temp;
	char	*name;
	char	*value;

	idx = 0;
	while (line[idx] != '\0')
	{
		if (line[idx] == '$' && line[idx + 1] != '\0' && line[idx + 1] != ' ')
		{
			temp = ++idx;
			while (line[idx] != '\0' && !(line[idx] == ' ' || line[idx] >= 9
					&& line[idx] <= 13) && line[idx] != '$')
				idx++;
			if (write_dollar_heredoc(line, temp, idx - temp, fd) == FALSE)
				return (FALSE);
		}
		else
		{
			ft_putchar_fd(line[idx], fd);
			idx++;
		}
	}
	write(fd, "\n", 1);
	return (TRUE);
}

void	move_heredoc_curser(int fd)
{
	close(fd);
	ft_putstr_fd("\x1b[1A", 1);
	ft_putstr_fd("\033[2C", 1);
	exit(0);
}
