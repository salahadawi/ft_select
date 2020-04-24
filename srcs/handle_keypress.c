/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keypress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:07:17 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:15:20 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		handle_keys(t_select *select)
{
	char c;

	c = read_key(select);
	if (c < 0)
		handle_control_sequence(select, &c);
	if (c == BACKSPACE)
	{
		if (!(delete_arg(select, select->current)))
			c = ESCAPE;
	}
	if (c == ESCAPE)
	{
		restore_terminal_mode(select);
		exit(0);
	}
	else if (c == ENTER)
		return (0);
	else if (c == SPACE)
		handle_space_key(select);
	else if (c == '+')
		select_deselect_all(select, 1);
	else if (c == '-')
		select_deselect_all(select, 0);
	return (1);
}

int		read_key(t_select *select)
{
	char c;
	char sequence[3];

	c = 0;
	if (read(STDERR_FILENO, &c, 1) == -1)
		handle_error(select, "Read failed", 1);
	if (c == ESCAPE)
	{
		if (read(STDERR_FILENO, &sequence[0], 1) != 1)
			return (ESCAPE);
		if (read(STDERR_FILENO, &sequence[1], 1) != 1)
			return (ESCAPE);
		if (sequence[0] == '[')
		{
			if (ft_isdigit(sequence[1]))
			{
				if (read(STDERR_FILENO, &sequence[2], 1) != 1)
					return (ESCAPE);
			}
			return (sequence[1] - 100);
		}
	}
	return (c);
}

void	handle_control_sequence(t_select *select, char *c)
{
	*c += 100;
	if (*c == select->key_sequences.delete)
	{
		if (!(delete_arg(select, select->current)))
			*c = ESCAPE;
	}
	else if (*c == select->key_sequences.left_arrow)
		select->current = select->current->prev;
	else if (*c == select->key_sequences.right_arrow)
		select->current = select->current->next;
	else if (*c == select->key_sequences.up_arrow)
		select->current = select->args;
	else if (*c == select->key_sequences.down_arrow)
		select->current = select->args->prev;
}

void	handle_space_key(t_select *select)
{
	select->current->selected = !select->current->selected;
	select->selected_amount += select->current->selected ? 1 : -1;
	select->current = select->current->next;
}
