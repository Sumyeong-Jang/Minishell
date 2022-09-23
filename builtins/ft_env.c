/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumjang <sumjang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:15:36 by sumjang           #+#    #+#             */
/*   Updated: 2022/09/23 14:15:36 by sumjang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env(void)
{
	int	i;

	i = 0;
	while (g_env_list.env_list[i])
	{
		printf("%s\n", g_env_list.env_list[i]);
		i++;
	}
}
