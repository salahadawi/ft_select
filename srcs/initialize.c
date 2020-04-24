/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:01:50 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:04:09 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	init_termcaps(t_select **select)
{
	char *terminal_name;

	*select = (t_select*)ft_memalloc(sizeof(t_select));
	(*select)->selected_amount = 0;
	if (!(isatty(0)))
		handle_error(*select, "Not a terminal", 0);
	if (!(terminal_name = getenv("TERM")))
		handle_error(*select, "Terminal enviroment variable not found", 0);
	if (tgetent(NULL, terminal_name) < 1)
		handle_error(*select, "Terminal specified in env not found", 0);
	init_key_sequences(*select);
	ioctl(STDERR_FILENO, TIOCGWINSZ, &(*select)->window_size);
}

void	init_key_sequences(t_select *select)
{
	char *sequence;

	sequence = tgetstr(LEFT_SEQUENCE, NULL);
	select->key_sequences.left_arrow = ft_strlen(sequence) > 2 ?
	sequence[2] : 0;
	sequence = tgetstr(RIGHT_SEQUENCE, NULL);
	select->key_sequences.right_arrow = ft_strlen(sequence) > 2 ?
	sequence[2] : 0;
	sequence = tgetstr(UP_SEQUENCE, NULL);
	select->key_sequences.up_arrow = ft_strlen(sequence) > 2 ?
	sequence[2] : 0;
	sequence = tgetstr(DOWN_SEQUENCE, NULL);
	select->key_sequences.down_arrow = ft_strlen(sequence) > 2 ?
	sequence[2] : 0;
	sequence = tgetstr(DELETE_SEQUENCE, NULL);
	select->key_sequences.delete = ft_strlen(sequence) > 2 ?
	sequence[2] : 0;
}

void	init_signal_handling(t_select *select)
{
	int i;

	g_select = select;
	i = 0;
	while (i <= SIGRTMAX)
		signal(i++, handle_signal);
}

void	init_args(t_select *select, int argc, char **argv)
{
	t_arg	*current;
	int		i;

	current = NULL;
	i = 1;
	while (i < argc)
	{
		if (!current)
		{
			current = new_arg(argv[i++], NULL);
			select->args = current;
			select->current = current;
		}
		else
		{
			current->next = new_arg(argv[i++], current);
			current = current->next;
		}
	}
	current->next = select->current;
	select->current->prev = current;
}

t_arg	*new_arg(char *str, t_arg *prev)
{
	t_arg *new;

	new = (t_arg*)ft_memalloc(sizeof(t_arg));
	new->str = str;
	new->selected = 0;
	new->prev = prev;
	new->next = NULL;
	return (new);
}
