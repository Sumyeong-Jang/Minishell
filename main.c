/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 14:46:30 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

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

static void	main_init(char **envp)
{
	g_st.env_list = copy_envs(envp);
	if (is_in_env_list("OLDPWD") == -1)
		edit_env_list(ft_strjoin(ft_strdup("OLDPWD="), get_pwd()));
	print_intro();
	set_main_signal();
}

static t_cmd_list	*cmd_init(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = (t_cmd_list *)malloc(sizeof(t_cmd_list));
	ft_memset(cmd_list, 0, sizeof(t_cmd_list));
	return (cmd_list);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd_list	*cmd_list;
	char		*line;

	main_init(envp);
	while (1)
	{
		echoctl_off();
		line = readline("minishell $ ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		cmd_list = cmd_init();
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
	free_split(g_st.env_list);
	return (0);
}
