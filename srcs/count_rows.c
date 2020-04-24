/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_rows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:10:23 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:10:39 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		count_rows(t_select *select)
{
	t_arg	*current;
	int		loop;
	int		line_len;
	int		rows;

	rows = 0;
	loop = 0;
	line_len = 0;
	current = select->args;
	while (current)
	{
		if (current == select->args)
			if (loop++)
				break ;
		if ((line_len += ft_strlen(current->str) + 2) >
		select->window_size.ws_col)
		{
			line_len = ft_strlen(current->str) + 2;
			rows++;
		}
		current = current->next;
	}
	return (rows);
}

int		screen_too_small(t_select *select)
{
	if (count_rows(select) >= select->window_size.ws_row)
	{
		print_screen_too_small(select);
		return (1);
	}
	return (0);
}
