/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:12:17 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		main(int argc, char **argv)
{
	t_select *select;

	init_termcaps(&select);
	tcgetattr(STDERR_FILENO, &select->old);
	if (argc < 2)
		handle_error(select, "Please include at least one argument", 0);
	init_signal_handling(select);
	init_args(select, argc, argv);
	create_terminal_raw_mode(select);
	set_terminal_raw_mode(select);
	while (1)
	{
		ft_clear_screen();
		print_args(select);
		if (!(handle_keys(select)))
			break ;
	}
	restore_terminal_mode(select);
	print_selected(select);
	free_memory(select);
	return (0);
}
