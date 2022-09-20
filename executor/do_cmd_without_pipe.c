#include "../includes/minishell.h"

void exec_cmd(t_cmd_list *cmd_list);
static void exec_without_pipe(t_cmd_node *node);
static void do_cmd_without_pipe(t_cmd_node *node);

void exec_cmd(t_cmd_list *cmd_list)
{
	if (cmd_list->size == 1)
		exec_without_pipe(cmd_list->cmd_heads[0]);
	else
		exec_with_pipe(cmd_list);
}

static void exec_without_pipe(t_cmd_node *node)
{
	pid_t pid;
	int status;

	if ((node->type == BUILTIN) && (have_redir_in(node) == NULL) && (have_redir_out(node) == NULL))
		exec_single_builtins(node);
	else
	{
		pid = fork();
		if (pid <= -1)
			exit(1); // exit_code 확인
		if (pid == 0)
		{
			do_cmd_without_pipe(node);
			exit(1);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (!WIFSIGNALED(status))
				g_env_list.exit_status = status / 256; //?
		}
	}
}

static void do_cmd_without_pipe(t_cmd_node *node)
{
	char **arg;
	char *tmp;
	t_cmd_node *cmd_list;

	redir_in(node);
	redir_out(node);
	cmd_list = remove_redir(node);
	if (cmd_list->type == BUILTIN)
		exec_builtins(cmd_list);
	tmp = is_valid_cmd(cmd_list);
	arg = string_array(cmd_list);
	if (execve(tmp, arg, g_env_list.env_list) == -1)
		execve_error(strerror(errno), cmd_list);
}