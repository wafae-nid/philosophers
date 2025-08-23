/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:47:54 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/23 01:05:55 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	atol(const char *str)
{
	long long	num;

	num = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\r' || *str == '\f')
		str++;
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if (num > INT_MAX)
			return (-1);
		num = num * 10 + (*str++ - 48);
	}
	return (num);
}

int	ft_is_a_numb(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	all_valid_chars(int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (j == 0 && (!ft_is_a_numb(argv[i][j]) && (argv[i][j] != '+')))
				return (0);
			else
			{
				if (j >= 1 && !ft_is_a_numb(argv[i][j]))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	all_checkings(int argc, char **argv)
{
	long	number;
	int		i;

	i = 1;
	if (all_valid_chars(argc, argv))
	{
		while (i < argc)
		{
			number = atol(argv[i]);
			if (number > INT_MAX || number <= 0)
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}
