/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjo <sjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 14:46:30 by sjo               #+#    #+#             */
/*   Updated: 2021/12/06 16:29:04 by sjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n)
	{
		i++;
		n /= 10;
	}	
	return (i);
}

char	*ft_itoa(int n)
{
	char		*result;
	int			len;
	long int	n_copy;

	len = ft_intlen(n);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = '\0';
	n_copy = (long int)n;
	if (n_copy < 0)
	{
		result[0] = '-';
		n_copy *= -1;
	}
	if (n_copy == 0)
		result[0] = '0';
	while (n_copy)
	{
		result[len - 1] = (n_copy % 10) + '0';
		n_copy /= 10;
		len--;
	}
	return (result);
}
