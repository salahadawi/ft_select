/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/20 16:06:05 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	handle_error(char *message)
{
	ft_printf("Error: %s.\r\n", message);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old);
	exit(0);
}

void	set_terminal_raw_mode(void)
{
	if (tcgetattr(0, &g_select->raw) == -1)
		handle_error("Invalid input/output descriptor");
	g_select->raw.c_lflag &= ~(ECHO | ICANON);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->raw) == -1)
		handle_error("Invalid input/output descriptor");
}

void	restore_terminal_mode(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
}

int		iscntrl(int c)
{
	return (c < 32 || c == 127);
}

void	handle_signal_suspend(void)
{
	signal(SIGTSTP, SIG_DFL);
	ioctl(0, TIOCSTI, "\032");
}

void	handle_signal_continue(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
	ft_printf("Raw terminal configuration restored!\n");
}

void	handle_signal_resize(void)
{

}

void	handle_signal(int sig)
{
	ft_printf("Signal %d handled!\n", sig);
	restore_terminal_mode();
	ft_printf("Old terminal configuration restored!\n");
	//free memory here
	if (sig == SIGTSTP)
		handle_signal_suspend();
	else if (sig == SIGCONT)
		handle_signal_continue();
	else if (sig == SIGWINCH)
		handle_signal_resize();
	else
		exit(0);
}

void	init_signal_handling(void)
{
	int i;

	i = 0;
	while (i <= SIGRTMAX)
		signal(i++, handle_signal);
}

int		main(int argc, char **argv)
{
	char c;

	init_signal_handling();
	g_select = (t_select*)ft_memalloc(sizeof(t_select));
	if (tcgetattr(0, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
	set_terminal_raw_mode();
	while (1)
	{
		if (read(0, &c, 1) == -1)
			handle_error("Read failed.");
		if (c == 'q')
			break;
		if (iscntrl(c))
			ft_printf("%d", c);
		else
			ft_printf("%c", c);
	}
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
	return (0);
}