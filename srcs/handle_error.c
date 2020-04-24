/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 20:04:55 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/24 20:05:05 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	handle_error(t_select *select, char *message, int reset)
{
	ft_fprintf(STDERR_FILENO, "Error: %s.\n", message);
	if (reset)
		restore_terminal_mode(select);
	free_memory(select);
	exit(0);
}
