/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/20 21:16:55 by sadawi           ###   ########.fr       */
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
	ft_printf("\x1b[?25h");
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

void	print_args(void)
{
	t_arg *current;

	current = g_select->args;
	while (current)
	{
		if (current->cursor)
			ft_printf(UNDERLINE);
		if (current->selected)
			ft_printf(INVERSE_VIDEO);
		ft_printf("%s" NORMAL, current->str);
		current = current->next;
		if (current)
			ft_printf(" ");
	}
}

void	ft_clear_screen(void)
{
	ft_printf("\x1b[?25l");
	ft_printf("\x1b[2J");
	ft_printf("\x1b[H");
}

int		ft_putschar(int c)
{
	write(1, &c, 1);
}

t_arg	*new_arg(char *str, t_arg *prev)
{
	t_arg *new;
	
	new = (t_arg*)ft_memalloc(sizeof(t_arg));
	new->str = str;
	new->selected = 0;
	if (!prev)
		new->cursor = 1;
	else
		new->cursor = 0;
	new->prev = prev;
	new->next = NULL;
	return (new);
}

void	init_args(int argc, char **argv)
{
	t_arg *current;
	int i;

	g_select = (t_select*)ft_memalloc(sizeof(t_select));
	current = NULL;
	i = 1;
	while (i < argc)
	{
		if (!current)
		{
			current = new_arg(argv[i++], NULL);
			g_select->args = current;
		}
		else
		{
			current->next = new_arg(argv[i++], current);
			current = current->next;
		}
	}
}

int		main(int argc, char **argv)
{
	char c;

	init_signal_handling();
	init_args(argc, argv);
	if (tcgetattr(0, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
	set_terminal_raw_mode();
	while (1)
	{
		ft_clear_screen();
		print_args();
		if (read(0, &c, 1) == -1)
			handle_error("Read failed.");
		if (c == 'q')
			break;
	}
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old) == -1)
		handle_error("Invalid input/output descriptor");
	return (0);
}