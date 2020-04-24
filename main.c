/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 16:15:00 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 13:20:52 by sadawi           ###   ########.fr       */
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

void	restore_terminal_mode(t_select *select)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &select->old);
	set_terminal(CURSOR_VISIBLE);
	set_terminal(TEXT_NORMAL);
	set_terminal(NORMAL_MODE);
}

void	handle_error(t_select *select, char *message, int reset)
{
	ft_printf("Error: %s.\n", message);
	if (reset)
		restore_terminal_mode(select);
	exit(0);
}

void	create_terminal_raw_mode(t_select *select)
{
	tcgetattr(0, &select->raw);
	select->raw.c_lflag &= ~(ECHO | ICANON);
	select->raw.c_cc[VMIN] = 0;
	select->raw.c_cc[VTIME] = 1;
}

void	set_terminal_raw_mode(t_select *select)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &select->raw);
	set_terminal(CURSOR_INVISIBLE);
	set_terminal(SPECIAL_MODE);
}

int		iscntrl(int c)
{
	return (c < 32 || c == 127);
}

void	handle_signal_suspend(t_select *select)
{
	restore_terminal_mode(select);
	signal(SIGTSTP, SIG_DFL);
	ioctl(0, TIOCSTI, "\032");
}

void	handle_signal_continue(t_select *select)
{
	set_terminal_raw_mode(select);
	//ft_printf("Raw terminal configuration restored!\n");
	ft_clear_screen(); // find way to not repeat this
	print_args(select);
}

void	handle_signal_resize(t_select *select)
{
	ioctl(0, TIOCGWINSZ, &select->window_size);
	ft_clear_screen();
	print_args(select);
	int i = 0;
	for (t_arg *arg = select->args->next; arg != select->args; arg = arg->next)
		i += ft_strlen(arg->str) + 1;
	i += ft_strlen(select->args->str);
	//ft_printf("%d\n", i);
	//ft_printf("%d\n", select->window_size.ws_col);
}

void	handle_signal(int sig)
{
	//ft_printf("Signal %d handled!\n", sig);
	//ft_printf("Old terminal configuration restored!\n");
	//free memory here
	if (sig == SIGTSTP)
		handle_signal_suspend(g_select);
	else if (sig == SIGCONT)
		handle_signal_continue(g_select);
	else if (sig == SIGWINCH)
		handle_signal_resize(g_select);
	else
	{
		restore_terminal_mode(g_select);
		exit(0);
	}
}

void	init_signal_handling(t_select *select)
{
	int i;

	g_select = select;
	i = 0;
	while (i <= SIGRTMAX)
		signal(i++, handle_signal);
}

int		count_rows(t_select *select)
{
	t_arg 	*current;
	int		loop;
	int		line_len;
	int		rows;

	rows = 0;
	loop = 0;
	line_len = 0;
	current = select->args;
	while (current)
	{
		if (current == select->args)
		{
			if (loop++)
				break;
		}
		if ((line_len += ft_strlen(current->str) + 2) > select->window_size.ws_col)
		{
			line_len = ft_strlen(current->str) + 2;
			rows++;
		}
		current = current->next;
	}
	return (rows);
}

void	print_screen_too_small(t_select *select)
{
	int i;
	char *message;

	i = 0;
	while (++i < select->window_size.ws_row / 2)
		ft_fprintf(0, "\n");
	i = 0;
	message = "Window is too small to fit all arguments.\n";
	while (i++ < select->window_size.ws_col / 2 - (int)ft_strlen(message) / 2)
		ft_fprintf(0, " ");
	ft_fprintf(0, "%s", message);
	i = 0;
	message = "Increase window size to continue.\n";
	while (i++ < select->window_size.ws_col / 2 - (int)ft_strlen(message) / 2)
		ft_fprintf(0, " ");
	ft_fprintf(0, "%s", message);
}

void	print_args(t_select *select)
{
	t_arg 	*current;
	int		loop;
	int		line_len;

	loop = 0;
	line_len = 0;
	current = select->args;
	if (count_rows(select) >= select->window_size.ws_row)
	{
		print_screen_too_small(select);
		return ;
	}
	while (current)
	{
		if (current == select->args)
		{
			if (loop++)
				break;
		}
		if (current == select->current)
			set_terminal(TEXT_UNDERLINE);
		if (current->selected)
			set_terminal(TEXT_INVERSE_VIDEO);
		if ((line_len += ft_strlen(current->str) + 2) > select->window_size.ws_col)
		{
			ft_fprintf(0, "\n");
			line_len = ft_strlen(current->str) + 2;
		}
		ft_fprintf(0, "%s", current->str);
		set_terminal(TEXT_NORMAL);
		current = current->next;
		if (current)
			ft_fprintf(0, "  ");
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
	new->prev = prev;
	new->next = NULL;
	return (new);
}

void	init_args(t_select *select, int argc, char **argv)
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
	ioctl(0, TIOCGWINSZ, &(*select)->window_size);
}

int		read_key(t_select *select)
{
	char c;
	char sequence[3];
	
	c = 0;
	if (read(0, &c, 1) == -1)
		handle_error(select, "Read failed.", 1);
	if (c == ESCAPE)
	{
		if (read(0, &sequence[0], 1) != 1)
			return (ESCAPE);
		if (read(0, &sequence[1], 1) != 1)
			return (ESCAPE);
		if (sequence[0] == '[')
		{
			if (ft_isdigit(sequence[1]))
			{
				if (read(0, &sequence[2], 1) != 1)
					return (ESCAPE);
			}
			return (sequence[1] - 100);
		}
	}
	return c;
}

int		delete_arg(t_select *select, t_arg *arg)
{
	int		last_arg;

	last_arg = 0;
	if (arg == arg->next)
		last_arg = 1;
	if (arg == select->args)
		select->args = select->args->next;
	arg->prev->next = arg->next;
	arg->next->prev = arg->prev;
	if (arg->selected)
		select->selected_amount--;
	select->current = arg->next;
	free(arg);
	arg = NULL;
	return (!last_arg);
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

void	select_deselect_all(t_select *select, int boolean)
{
	t_arg	*arg;
	int		loop;

	arg = select->args;
	select->selected_amount = 0;
	loop = 0;
	while(arg)
	{
		if (arg == select->args)
			if (loop++)
				break;
		arg->selected = boolean;
		select->selected_amount += boolean;
		arg = arg->next;
	}
}

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
	{
		select->current->selected = !select->current->selected;
		select->selected_amount += select->current->selected ? 1 : -1;
		select->current = select->current->next;
	}
	else if (c == '+')
		select_deselect_all(select, 1);
	else if (c == '-')
		select_deselect_all(select, 0);
	return (1);
}

void	print_selected(t_select *select)
{
	t_arg 	*current;
	int		loop;

	loop = 0;
	current = select->args;
	while (current)
	{
		if (current == select->args)
			if (loop++)
				break;
		if (current->selected)
		{
			ft_fprintf(1, "%s", current->str);
			if (select->selected_amount-- > 1)
				ft_fprintf(1, " ");
		}
		current = current->next;
	}
}

int		main(int argc, char **argv)
{
	t_select *select;

	init_termcaps(&select);
	tcgetattr(0, &select->old);
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
			break;
	}
	restore_terminal_mode(select);
	print_selected(select);
	return (0);
}