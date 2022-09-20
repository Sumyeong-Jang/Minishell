#include "../includes/minishell.h"

void	ft_unset(t_cmd_node *head);
static int	ft_unset_2(t_cmd_node *head, int flag);
static int	unset_error(char *cmd);

void	ft_unset(t_cmd_node *head)
{
	int			flag;

	flag = ft_unset_2(head, FALSE);
	if (flag == TRUE)
		exit(1);
}

static int	ft_unset_2(t_cmd_node *head, int flag)
{
	t_cmd_node	*curr_node;
	int			loc;

	curr_node = head->next;
	while (curr_node != NULL)
	{
		if (is_right_form(curr_node->cmd) == FALSE)
			flag = unset_error(curr_node->cmd);
		curr_node = curr_node->next;
	}
	return (flag);
}

static int	unset_error(char *cmd)
{
	ft_putstr_fd("bash : unset : ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	return (TRUE);
}