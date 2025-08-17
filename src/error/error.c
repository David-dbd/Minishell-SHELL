/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:26:20 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:00:35 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	syntax_error(t_shell *data, char *error, int exit_code)
{
	if (data->prompt.tokens)
		clean_prompt(&data->prompt);
	if (error)
		ft_printf_fd(STDERR, error);
	data->last_exit_code = exit_code;
	data->prompt.error = TRUE;
	return (exit_code);
}
