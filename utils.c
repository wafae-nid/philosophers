/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:15:38 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/17 20:28:32 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_input_error(int flag)
{
	if (flag == 0)
		printf("arguments should be 5 or optionally 6 ....try again :) \n");
	else 
		printf("arguments should be all positive numbers and less than int max .... try again:) \n");
	exit(EXIT_FAILURE);
}