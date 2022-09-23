/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 14:46:30 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 12:21:44 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

static void	echoctl_off(void)
{
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &attr);
	attr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

static void	echoctl_on(void)
{
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &attr);
	attr.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}

static void	print_intro(void)
{
	int		fd;
	char	c;

	fd = open("intro.txt", O_RDONLY);
	while (read(fd, &c, 1) > 0)
		ft_putchar_fd(c, STDOUT_FILENO);
	ft_putendl_fd("", STDOUT_FILENO);
	close(fd);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd_list	*cmd_list;
	char		*line;

	g_env_list.env_list = copy_envs(envp);
	if (is_in_env_list("OLDPWD") == -1)
		edit_env_list(ft_strjoin(ft_strdup("OLDPWD="), get_pwd()));
	print_intro();
	set_main_signal();
	while (1)
	{
		echoctl_off();
		line = readline("minishell $ ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		cmd_list = (t_cmd_list *)malloc(sizeof(t_cmd_list));
		if (!cmd_list)
			return (0);
		ft_memset(cmd_list, 0, sizeof(t_cmd_list));
		if (parse_cmd(line, &cmd_list) == 0)
		{
			free_cmd(cmd_list);
			continue ;
		}
		echoctl_on();
		exec_cmd(cmd_list);
		free_cmd(cmd_list);
		remove_temp_file();
	}
	free_split(g_env_list.env_list);
	return (0);
}
