/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:50 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		have_equal(char *str);
int		is_in_envp(char *str);
void	modify_envp(char *str, int loc);
char	**new_export(char *str);

int	have_equal(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '=')
			return (TRUE);
	}
	return (FALSE);
}

int	is_in_envp(char *str)
{
	char	**split;
	int		i;

	i = -1;
	split = ft_split(str, '=');
	while (g_st.env_list[++i])
	{
		if (ft_strncmp(g_st.env_list[i], split[0], \
		ft_strlen(split[0])) == 0)
		{
			free_split(split);
			return (i);
		}
	}
	free_split(split);
	return (-1);
}

char	**new_export(char *str)
{
	int		i;
	int		cnt;
	char	**ret;

	i = -1;
	cnt = 0;
	while (g_st.env_list[++i])
		cnt++;
	ret = (char **)malloc(sizeof(char *) * (cnt + 2));
	if (!ret)
		exit(1);
	ret[cnt + 1] = NULL;
	i = -1;
	while (g_st.env_list[++i])
		ret[i] = ft_strdup(g_st.env_list[i]);
	ret[cnt] = ft_strdup(str);
	ret[cnt + 1] = NULL;
	free_split(g_st.env_list);
	return (ret);
}
