/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:08 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_cd(t_cmd_node *head);
char	*get_value(char *key);
int		key_len(char *str);
int		value_len(char *str);
void	home_dir(char *str);

void	ft_cd(t_cmd_node *head)
{
	t_cmd_node	*curr_node;
	int			ret;
	char		*str;

	curr_node = head->next;
	ret = 0;
	str = get_value("HOME");
	if (curr_node == NULL)
		home_dir(str);
	else if (ft_strcmp(curr_node->cmd, "~") == 0)
		home_dir(str);
	else if (ft_strcmp(curr_node->cmd, "-") == 0)
	{
		free(str);
		old_dir();
	}
	else
	{
		free(str);
		change_dir(curr_node->cmd);
	}
}

char	*get_value(char *key)
{
	int		i;
	char	*ret;

	i = -1;
	ret = NULL;
	while (g_st.env_list[++i])
	{
		if ((ft_strncmp(key, g_st.env_list[i], ft_strlen(key)) == 0) \
		&& g_st.env_list[i][ft_strlen(key)] == '=')
			ret = ft_substr(g_st.env_list[i], key_len(g_st.\
			env_list[i]) + 1, value_len(g_st.env_list[i]));
	}
	return (ret);
}

int	key_len(char *str)
{
	int	ret;
	int	i;

	ret = -1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			return (ret + 1);
		ret++;
	}
	return (ret);
}

int	value_len(char *str)
{
	int	ret;
	int	i;

	ret = -1;
	i = -1;
	ret = ft_strlen(str) - key_len(str) - 1;
	return (ret);
}

void	home_dir(char *str)
{
	char	*tmp1;

	tmp1 = ft_strdup("PWD=");
	if (chdir(str) < 0)
	{
		free(str);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(1);
	}
	tmp1 = ft_strjoin(tmp1, str);
	export_str(tmp1);
	free(tmp1);
	tmp1 = 0;
}
