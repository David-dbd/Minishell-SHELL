/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:26:20 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 18:11:14 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	syntax_error(t_shell *data, const char *error, int exit_code, ...)
{
	va_list args;

	if (data->prompt.tokens)
		clean_prompt(&data->prompt);
	if (error)
	{
		va_start(args, exit_code);
		ft_var_printf_fd(STDERR, error, args);
		va_end(args);
	}
	data->last_exit_code = exit_code;
	data->prompt.error = TRUE;
	return (exit_code);
}
