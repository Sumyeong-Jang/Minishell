#include "../includes/minishell.h"

void ft_env(void)
{
	int i;

	i = 0;
	while (g_env_list.env_list[i])
	{
		printf("%s\n", g_env_list.env_list[i]);
		i++;
	}
}
