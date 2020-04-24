/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:06:12 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:06:40 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	handle_signal(int sig)
{
	if (sig == SIGTSTP)
		handle_signal_suspend(g_select);
	else if (sig == SIGCONT)
		handle_signal_continue(g_select);
	else if (sig == SIGWINCH)
		handle_signal_resize(g_select);
	else
	{
		restore_terminal_mode(g_select);
		free_memory(g_select);
		exit(0);
	}
}

void	handle_signal_suspend(t_select *select)
{
	restore_terminal_mode(select);
	signal(SIGTSTP, SIG_DFL);
	ioctl(STDERR_FILENO, TIOCSTI, "\032");
}

void	handle_signal_continue(t_select *select)
{
	set_terminal_raw_mode(select);
	signal(SIGTSTP, handle_signal);
	ft_clear_screen();
	print_args(select);
}

void	handle_signal_resize(t_select *select)
{
	ioctl(STDERR_FILENO, TIOCGWINSZ, &select->window_size);
	ft_clear_screen();
	print_args(select);
}
