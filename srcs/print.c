/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:08:15 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:15:39 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	print_args(t_select *select)
{
	t_arg	*current;
	int		loop;
	int		line_len;

	line_len = 0;
	current = select->args;
	if (screen_too_small(select))
		return ;
	loop = 0;
	while (current)
	{
		if (current == select->args)
			if (loop++)
				break ;
		if ((line_len += ft_strlen(current->str) + 2) >
		select->window_size.ws_col)
		{
			ft_fprintf(STDERR_FILENO, "\n");
			line_len = ft_strlen(current->str) + 2;
		}
		print_arg(select, current);
		current = current->next;
		if (current)
			ft_fprintf(STDERR_FILENO, "  ");
	}
}

void	print_arg(t_select *select, t_arg *arg)
{
	struct stat stats;

	if (arg == select->current)
		set_terminal(TEXT_UNDERLINE);
	if (arg->selected)
		set_terminal(TEXT_INVERSE_VIDEO);
	lstat(arg->str, &stats);
	if (S_ISDIR(stats.st_mode))
		ft_printf("\033[1;34m");
	if (!S_ISDIR(stats.st_mode) &&
		stats.st_mode & S_IXUSR)
		ft_printf("\033[1;32m");
	if (S_ISLNK(stats.st_mode))
		ft_printf("\033[1;31m");
	ft_fprintf(STDERR_FILENO, "%s", arg->str);
	set_terminal(TEXT_NORMAL);
}

void	print_screen_too_small(t_select *select)
{
	int		i;
	char	*message;

	i = 0;
	while (++i < select->window_size.ws_row / 2)
		ft_fprintf(STDERR_FILENO, "\n");
	i = 0;
	message = "Window is too small to fit all arguments.\n";
	while (i++ < select->window_size.ws_col / 2 - (int)ft_strlen(message) / 2)
		ft_fprintf(STDERR_FILENO, " ");
	ft_fprintf(STDERR_FILENO, "%s", message);
	i = 0;
	message = "Increase window size to continue.\n";
	while (i++ < select->window_size.ws_col / 2 - (int)ft_strlen(message) / 2)
		ft_fprintf(STDERR_FILENO, " ");
	ft_fprintf(STDERR_FILENO, "%s", message);
}

void	print_selected(t_select *select)
{
	t_arg	*current;
	int		loop;

	loop = 0;
	current = select->args;
	while (current)
	{
		if (current == select->args)
			if (loop++)
				break ;
		if (current->selected)
		{
			ft_fprintf(STDOUT_FILENO, "%s", current->str);
			if (select->selected_amount-- > 1)
				ft_fprintf(STDOUT_FILENO, " ");
		}
		current = current->next;
	}
}
