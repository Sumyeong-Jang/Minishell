#include "../includes/minishell.h"

void	ft_pwd(void)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);

	if (current_dir == NULL)
		printf("getcwd() cannot excute in fun_pwd\n");//? 출력?
	else
	{
		printf("%s\n", current_dir);
		free(current_dir);
	}
}