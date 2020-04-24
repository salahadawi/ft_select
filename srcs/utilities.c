/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:11:36 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:11:47 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	ft_clear_screen(void)
{
	set_terminal(CLEAR_SCREEN);
}

int		delete_arg(t_select *select, t_arg *arg)
{
	int		last_arg;

	last_arg = 0;
	if (arg == arg->next)
		last_arg = 1;
	if (arg == select->args)
		select->args = select->args->next;
	arg->prev->next = arg->next;
	arg->next->prev = arg->prev;
	if (arg->selected)
		select->selected_amount--;
	select->current = arg->next;
	free(arg);
	arg = NULL;
	return (!last_arg);
}

void	select_deselect_all(t_select *select, int boolean)
{
	t_arg	*arg;
	int		loop;

	arg = select->args;
	select->selected_amount = 0;
	loop = 0;
	while (arg)
	{
		if (arg == select->args)
			if (loop++)
				break ;
		arg->selected = boolean;
		select->selected_amount += boolean;
		arg = arg->next;
	}
}
