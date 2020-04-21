/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/10 12:20:24 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/21 20:13:19 by sadawi           ###   ########.fr       */
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

# define ENTER 10
# define ESCAPE 27
# define SPACE 32
# define LEFT_ARROW -32
# define RIGHT_ARROW -33

typedef	struct		s_arg
{
	char			*str;
	int				selected;
	int				cursor;
	struct s_arg	*prev;
	struct s_arg	*next;
}					t_arg;

typedef struct		s_select
{
	struct termios	old;
	struct termios	raw;
	t_arg			*args;
	t_arg			*current;
}					t_select;

t_select			*g_select;

void	ft_clear_screen(void);

void	print_args(void);

#endif
