/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:28:49 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/23 01:23:33 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_global_garbage(t_trash **trash)
{
	t_trash	*tmp;

	while (*trash)
	{
		tmp = *trash;
		*trash = (*trash)->next;
		free(tmp->point);
		free(tmp);
	}
}

static void	*collection(t_trash **trash, size_t size)
{
	t_trash	*new;
	void	*ptr;

	new = malloc(sizeof(t_trash));
	if (!new)
	{
		free_global_garbage(trash);
		return (NULL);
	}
	ptr = malloc(size);
	if (!ptr)
	{
		free(new);
		free_global_garbage(trash);
		return (NULL);
	}
	new->point = ptr;
	new->next = *trash;
	*trash = new;
	return (ptr);
}

void	*gc_malloc(size_t size)
{
	static t_trash	*trash;
	void			*ptr;

	ptr = NULL;
	if (size)
		ptr = collection(&trash, size);
	else
		free_global_garbage(&trash);
	return (ptr);
}
