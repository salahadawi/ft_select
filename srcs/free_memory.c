/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:04:23 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:04:38 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	free_memory(t_select *select)
{
	t_arg *next;

	if (select->args)
	{
		select->args->prev->next = NULL;
		while (select->args)
		{
			select->args->prev = NULL;
			next = select->args->next;
			free(select->args);
			select->args = next;
		}
		free(select->args);
	}
	free(select);
}
