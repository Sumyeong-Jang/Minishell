#include "../includes/minishell.h"

void	remove_temp_file(void)
{
	struct stat	s;

	if (stat("ejang.jeyoon", &s) == 0)
		unlink("ejang.jeyoon");
}

void	move_heredoc_curser(int fd)
{
	close(fd);
	ft_putstr_fd("\x1b[1A", 1);
	ft_putstr_fd("\033[2C", 1);
	exit(0);
}