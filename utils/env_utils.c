/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 15:37:27 by sjo               #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_envs(char **envp)
{
	int		i;
	char	**ret;
	char	*tmp;
	char	*pwd;

	i = 0;
	while (envp[i])
		i++;
	ret = (char **)malloc(sizeof(char *) * (i + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		ret[i] = ft_strdup(envp[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

int	is_in_env_list(char *str)
{
	char	**split;
	int		i;

	i = 0;
	split = ft_split(str, '=');
	while (g_st.env_list[i])
	{
		if (ft_strncmp(g_st.env_list[i], split[0], \
		ft_strlen(split[0])) == 0)
		{
			ft_free(split);
			return (i);
		}
		i++;
	}
	ft_free(split);
	return (-1);
}

void	modify_envp(char *str, int i)
{
	free(g_st.env_list[i]);
	g_st.env_list[i] = ft_strdup(str);
}

char	**add_env(char *str)
{
	int		i;
	char	**ret;

	i = 0;
	while (g_st.env_list[i])
		i++;
	ret = (char **)malloc(sizeof(char *) * (i + 2));
	if (!ret)
		exit(1);
	ret[i + 1] = NULL;
	i = 0;
	while (g_st.env_list[i])
	{
		ret[i] = ft_strdup(g_st.env_list[i]);
		i++;
	}
	ret[i] = ft_strdup(str);
	ret[i + 1] = NULL;
	ft_free(g_st.env_list);
	return (ret);
}

void	edit_env_list(char *str)
{
	if (is_in_env_list(str) != -1)
		modify_envp(str, is_in_envp(str));
	else
		g_st.env_list = add_env(str);
}
