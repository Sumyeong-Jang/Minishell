#include "../includes/minishell.h"

void	ft_echo(t_cmd_node *head);
static void	print_echo(t_cmd_node *curr, int flag);

void	ft_echo(t_cmd_node *head)
{
	t_cmd_node	*curr_node;
	int			flag;

	curr_node = head->next;
	if (curr_node == NULL)
	{
		printf("\n");
		g_env_list.exit_status = 0;
		return ;
	}
	if (curr_node->type == OPTION)
	{
		flag = TRUE;
		curr_node = curr_node->next;
	}
	else
		flag = FALSE;
	g_env_list.exit_status = 0;
	if (curr_node == NULL)
		return ;
	print_echo(curr_node, flag);
}

static void	print_echo(t_cmd_node *curr, int flag)
{
	t_cmd_node	*curr_node;

	curr_node = curr;
	while (curr_node->next != NULL)
	{
		printf("%s ", curr_node->cmd);
		curr_node = curr_node->next;
	}
	printf("%s", curr_node->cmd);
	if (flag == FALSE)
		printf("\n");
}