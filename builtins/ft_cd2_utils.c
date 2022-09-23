/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:17 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 22:23:15 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_str(char *str);
void	old_dir(void);
void	change_dir(char *str);
char	*get_pwd(void);

void	export_str(char *str)
{
	if (is_in_envp(str) != -1)
		modify_envp(str, is_in_envp(str));
	else
		g_st.env_list = new_export(str);
}

void	old_dir(void)
{
	char	*str1;
	char	*str2;
	char	*tmp;

	str1 = get_value("OLDPWD");
	str2 = get_value("PWD");
	if (chdir(str1) < 0)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit(1);
	}
	tmp = ft_strdup("PWD=");
	tmp = ft_strjoin(tmp, str1);
	export_str(tmp);
	free(tmp);
	tmp = ft_strdup("OLDPWD=");
	tmp = ft_strjoin(tmp, str2);
	export_str(tmp);
	free(tmp);
	tmp = 0;
}

void	change_dir(char *str)
{
	char	*tmp;
	char	*ret;

	tmp = get_value("PWD");
	if (chdir(str) < 0)
	{
		free(tmp);
		printf("cd: no such file or directory: %s\n", str);
		exit(1);
	}
	else
	{
		ret = ft_strdup("PWD=");
		ret = ft_strjoin(ret, get_pwd());
		export_str(ret);
		free(ret);
		ret = ft_strdup("OLDPWD=");
		ret = ft_strjoin(ret, tmp);
		export_str(ret);
		free(ret);
		ret = 0;
	}
}
