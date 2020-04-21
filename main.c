/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/21 14:59:00 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		ft_putschar(int c)
{
	if (write(0, &c, 1) == -1)
		return (-1);
	return (0);
}

void	set_terminal(char *id)
{
	tputs(tgetstr(id, NULL), 1, ft_putschar);
}

void	restore_terminal_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->old);
	set_terminal(CURSOR_VISIBLE);
}

void	handle_error(char *message)
{
	ft_printf("Error: %s.\n", message);
	restore_terminal_mode();
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
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->raw) == -1)
		handle_error("Invalid input/output descriptor");
	ft_printf("Raw terminal configuration restored!\n");
	ft_clear_screen(); // find way to not repeat this
	print_args();
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
			set_terminal(TEXT_UNDERLINE);
		if (current->selected)
			set_terminal(TEXT_INVERSE_VIDEO);
		ft_fprintf(0, "%s", current->str);
		set_terminal(TEXT_NORMAL);
		current = current->next;
		if (current)
			ft_fprintf(0, " ");
	}
}

void	ft_clear_screen(void)
{
	set_terminal(CURSOR_INVISIBLE);
	set_terminal(CLEAR_SCREEN);
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

void	init_termcaps(void)
{
	char *terminal_name;

	if (!(isatty(0)))
		handle_error("Not a terminal");
	if (!(terminal_name = getenv("TERM")))
		handle_error("Terminal enviroment variable not found");
	if (!(tgetent(NULL, terminal_name)))
		handle_error("Terminal specified in env not found");
		
}

int		main(int argc, char **argv)
{
	char c;

	init_signal_handling();
	init_args(argc, argv);
	tcgetattr(0, &g_select->old);
	init_termcaps();
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
	restore_terminal_mode();
	return (0);
}