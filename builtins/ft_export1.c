#include "../includes/minishell.h"

void ft_export(t_cmd_node *head);
void ft_export_single_cmd(t_cmd_node *head);
void export_wihtout_arg(t_cmd_node *head);
int is_right_form(char *str);

void ft_export(t_cmd_node *head)
{
	int flag;
	t_cmd_node *curr_node;

	curr_node = head->next;
	flag = FALSE;
	export_wihtout_arg(head);
	while (curr_node != NULL)
	{
		if (is_right_form(curr_node->cmd) == FALSE)
		{
			flag = TRUE;
			ft_putstr_fd("bash : export : ", STDERR_FILENO);
			ft_putstr_fd(curr_node->cmd, STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		}
		curr_node = curr_node->next;
	}
	if (flag == TRUE)
		exit(1);
}

void ft_export_single_cmd(t_cmd_node *head)
{
	int idx;
	t_cmd_node *curr_node;

	curr_node = head->next;
	export_wihtout_arg(head);
	while (curr_node != NULL)
	{
		if (is_right_form(curr_node->cmd) == FALSE)
		{
			ft_putstr_fd("bash : export : ", STDERR_FILENO);
			ft_putstr_fd(curr_node->cmd, STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			g_env_list.exit_status = 1;
		}
		else if ((has_equal_sign(curr_node->cmd) == TRUE))
		{
			idx = is_in_envp(curr_node->cmd);
			if (idx != -1)
				modify_envp(curr_node->cmd, idx);
			else
				g_env_list.env_list = new_export(curr_node->cmd);
		}
		curr_node = curr_node->next;
	}
}

void export_wihtout_arg(t_cmd_node *head)
{
	int i;

	i = 0;
	if (head->next == NULL)
	{
		while (g_env_list.env_list[i])
		{
			printf("declare -x %s\n", g_env_list.env_list[i]);
			i++;
		}
	}
}