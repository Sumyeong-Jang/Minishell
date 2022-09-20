/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 18:26:02 by sjo               #+#    #+#             */
/*   Updated: 2021/12/03 15:00:08 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*new_s;
	unsigned char	val;

	new_s = (unsigned char *)s;
	val = c;
	while (n--)
	{
		if (*new_s++ == val)
			return ((void *)(new_s - 1));
	}
	return (0);
}
