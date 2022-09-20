#include "../includes/minishell.h"

void	ft_env(void)
{
	int	i;

	i = 0;
	while (g_env_list.envp[i])
	{
		printf("%s\n", g_env_list.envp[i]);
		i++;
	}
}