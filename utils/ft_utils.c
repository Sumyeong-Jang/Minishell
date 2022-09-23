/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 05:16:55 by sjo               #+#    #+#             */
/*   Updated: 2022/09/21 05:17:29 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	tolower_str(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] != '\0')
	{
		str[idx] = ft_tolower(str[idx]);
		idx++;
	}
}

char	*get_lower_str(char *str)
{
	int		idx;
	int		len;
	char	*ret;

	len = ft_strlen(str);
	if (len == 0)
		return (ft_strdup(""));
	ret = ft_strdup(str);
	if (ret == 0)
		return (0);
	tolower_str(ret);
	return (ret);
}
