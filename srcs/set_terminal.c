/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_terminal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:05:20 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:05:53 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	set_terminal(char *id)
{
	tputs(tgetstr(id, NULL), 1, ft_putschar);
}

int		ft_putschar(int c)
{
	if (write(STDERR_FILENO, &c, 1) == -1)
		return (-1);
	return (0);
}

void	restore_terminal_mode(t_select *select)
{
	tcsetattr(STDERR_FILENO, TCSAFLUSH, &select->old);
	set_terminal(CURSOR_VISIBLE);
	set_terminal(TEXT_NORMAL);
	set_terminal(NORMAL_MODE);
}

void	create_terminal_raw_mode(t_select *select)
{
	tcgetattr(STDERR_FILENO, &select->raw);
	select->raw.c_lflag &= ~(ECHO | ICANON);
	select->raw.c_cc[VMIN] = 0;
	select->raw.c_cc[VTIME] = 1;
}

void	set_terminal_raw_mode(t_select *select)
{
	tcsetattr(STDERR_FILENO, TCSAFLUSH, &select->raw);
	set_terminal(CURSOR_INVISIBLE);
	set_terminal(SPECIAL_MODE);
}
