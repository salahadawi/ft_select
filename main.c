/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/22 19:24:45 by sadawi           ###   ########.fr       */
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
	set_terminal(TEXT_NORMAL);
	set_terminal("te");
}

void	handle_error(char *message)
{
	ft_printf("Error: %s.\n", message);
	restore_terminal_mode();
	exit(0);
}

void	create_terminal_raw_mode(void)
{
	tcgetattr(0, &g_select->raw);
	g_select->raw.c_lflag &= ~(ECHO | ICANON);
	g_select->raw.c_cc[VMIN] = 0;
	g_select->raw.c_cc[VTIME] = 1;
}

void	set_terminal_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_select->raw);
	set_terminal(CURSOR_INVISIBLE);
	set_terminal("ti");
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
	set_terminal_raw_mode();
	//ft_printf("Raw terminal configuration restored!\n");
	ft_clear_screen(); // find way to not repeat this
	print_args();
}

void	handle_signal_resize(void)
{

}

void	handle_signal(int sig)
{
	//ft_printf("Signal %d handled!\n", sig);
	if (sig != SIGCONT)
		restore_terminal_mode();
	//ft_printf("Old terminal configuration restored!\n");
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
	t_arg 	*current;
	int		loop;

	loop = 0;
	current = g_select->args;
	while (current)
	{
		if (current == g_select->args)
		{
			if (loop++)
				break;
		}
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

	current = NULL;
	i = 1;
	while (i < argc)
	{
		if (!current)
		{
			current = new_arg(argv[i++], NULL);
			g_select->args = current;
			g_select->current = current;
		}
		else
		{
			current->next = new_arg(argv[i++], current);
			current = current->next;
		}
	}
	current->next = g_select->current;
	g_select->current->prev = current;
}

void	init_termcaps(void)
{
	char *terminal_name;

	g_select = (t_select*)ft_memalloc(sizeof(t_select));
	g_select->selected_amount = 0;
	if (!(isatty(0)))
		handle_error("Not a terminal");
	if (!(terminal_name = getenv("TERM")))
		handle_error("Terminal enviroment variable not found");
	if (tgetent(NULL, terminal_name) < 1)
		handle_error("Terminal specified in env not found");
}

int		read_key(void)
{
	char c;
	char sequence[3];
	
	c = 0;
	if (read(0, &c, 1) == -1)
		handle_error("Read failed.");
	if (c == 27)
	{
		if (read(0, &sequence[0], 1) != 1)
			return (27);
		if (read(0, &sequence[1], 1) != 1)
			return (27);
		if (sequence[0] == '[')
			return (sequence[1] - 100);
	}
	return c;
}

int		handle_keys(void)
{
	char c;

	c = read_key();
	if (c == ESCAPE)
	{
		//ft_printf("\n");
		restore_terminal_mode();
		exit(0);
	}
	else if (c == ENTER)
		return (0);
	else if (c == SPACE)
	{
		g_select->current->selected = !g_select->current->selected;
		g_select->selected_amount += g_select->current->selected ? 1 : -1;
		c = RIGHT_ARROW;
	}
	else if (c == LEFT_ARROW)
	{
		g_select->current->cursor = 0;
		g_select->current = g_select->current->prev;
		g_select->current->cursor = 1;
	}
	if (c == RIGHT_ARROW)
	{
		g_select->current->cursor = 0;
		g_select->current = g_select->current->next;
		g_select->current->cursor = 1;
	}
	return (1);
}

void	print_selected(void)
{
	t_arg 	*current;
	int		loop;

	loop = 0;
	current = g_select->args;
	while (current)
	{
		if (current == g_select->args)
		{
			if (loop++)
				break;
		}
		if (current->selected)
		{
			ft_fprintf(1, "%s", current->str);
			if (g_select->selected_amount-- > 1)
				ft_fprintf(1, " ");
		}
		current = current->next;
	}
	//ft_printf("\n");
}

int		main(int argc, char **argv)
{
	char c;

	init_termcaps();
	tcgetattr(0, &g_select->old);
	if (argc < 2)
		handle_error("Please include at least one argument");
	init_signal_handling();
	init_args(argc, argv);
	create_terminal_raw_mode();
	set_terminal_raw_mode();
	while (1)
	{
		ft_clear_screen();
		print_args();
		if (!(handle_keys()))
			break;
	}
	restore_terminal_mode();
	print_selected();
	return (0);
}