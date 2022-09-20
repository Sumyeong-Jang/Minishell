/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:56:45 by sjo               #+#    #+#             */
/*   Updated: 2021/12/06 16:04:55 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	isincluded(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i++] == c)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	char	*start;
	char	*end;
	size_t	len;

	if (s1 == 0 || set == 0)
		return (NULL);
	start = (char *)s1;
	end = start + ft_strlen(s1);
	while (*start && isincluded (*start, set))
		start++;
	while (start < end && isincluded (*(end - 1), set))
		end--;
	len = end - start + 1;
	ret = (char *)malloc(len);
	if (!ret)
		return (0);
	ft_strlcpy(ret, start, len);
	return (ret);
}
