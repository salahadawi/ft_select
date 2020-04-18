/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/18 22:33:18 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	handle_error(t_select *select, char *message)
{
	ft_printf("Error: %s.\r\n", message);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &select->old);
	exit(0);
}

void	terminal_raw_mode(t_select *select)
{
	struct termios raw;

	if (tcgetattr(0, &raw) == -1)
		handle_error(select, "Invalid input/output descriptor");
	raw.c_oflag &= ~(OPOST);
	raw.c_iflag &= ~(IXON | ICRNL);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | VSUSP);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		handle_error(select, "Invalid input/output descriptor");
}

int		iscntrl(int c)
{
	return (c < 32 || c == 127);
}

int		main(int argc, char **argv)
{
	t_select *select;
	char c;

	select = (t_select*)ft_memalloc(sizeof(t_select));
	if (tcgetattr(0, &select->old) == -1)
		handle_error(select, "Invalid input/output descriptor");
	terminal_raw_mode(select);
	while (1)
	{
		if (read(0, &c, 1) == -1)
			handle_error(select, "Read failed.");
		if (c == 'q')
			break;
		if (iscntrl(c))
			ft_printf("%d", c);
		else
			ft_printf("%c", c);
	}
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &select->old) == -1)
		handle_error(select, "Invalid input/output descriptor");
	return (0);
}