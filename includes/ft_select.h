/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/10 12:20:24 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:16:00 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include "../libft/includes/libft.h"
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <termios.h>
# include <termcap.h>
# include <signal.h>
# include <fcntl.h>

# define TEXT_INVERSE_VIDEO "mr"
# define TEXT_UNDERLINE "us"
# define TEXT_NORMAL "me"
# define CURSOR_INVISIBLE "vi"
# define CURSOR_VISIBLE "ve"
# define CLEAR_SCREEN "cl"
# define LEFT_SEQUENCE "kl"
# define RIGHT_SEQUENCE "kr"
# define UP_SEQUENCE "ku"
# define DOWN_SEQUENCE "kd"
# define DELETE_SEQUENCE "kD"
# define SPECIAL_MODE "ti"
# define NORMAL_MODE "te"

# define DELETE -49
# define ENTER 10
# define ESCAPE 27
# define SPACE 32
# define BACKSPACE 127

typedef	struct		s_arg
{
	char			*str;
	int				selected;
	struct s_arg	*prev;
	struct s_arg	*next;
}					t_arg;

typedef	struct		s_key_sequences
{
	char			left_arrow;
	char			right_arrow;
	char			up_arrow;
	char			down_arrow;
	char			delete;
}					t_key_sequences;

typedef struct		s_select
{
	struct termios	old;
	struct termios	raw;
	t_key_sequences	key_sequences;
	t_arg			*args;
	t_arg			*current;
	int				selected_amount;
	struct winsize	window_size;
}					t_select;

t_select			*g_select;

int					count_rows(t_select *select);

int					screen_too_small(t_select *select);

void				free_memory(t_select *select);

void				handle_error(t_select *select, char *message, int reset);

int					handle_keys(t_select *select);

int					read_key(t_select *select);

void				handle_control_sequence(t_select *select, char *c);

void				handle_space_key(t_select *select);

void				handle_signal(int sig);

void				handle_signal_suspend(t_select *select);

void				handle_signal_continue(t_select *select);

void				handle_signal_resize(t_select *select);

void				init_termcaps(t_select **select);

void				init_key_sequences(t_select *select);

void				init_signal_handling(t_select *select);

void				init_args(t_select *select, int argc, char **argv);

t_arg				*new_arg(char *str, t_arg *prev);

void				print_args(t_select *select);

void				print_arg(t_select *select, t_arg *arg);

void				print_screen_too_small(t_select *select);

void				print_selected(t_select *select);

void				set_terminal(char *id);

int					ft_putschar(int c);

void				restore_terminal_mode(t_select *select);

void				create_terminal_raw_mode(t_select *select);

void				set_terminal_raw_mode(t_select *select);

void				ft_clear_screen(void);

int					delete_arg(t_select *select, t_arg *arg);

void				select_deselect_all(t_select *select, int boolean);

#endif
