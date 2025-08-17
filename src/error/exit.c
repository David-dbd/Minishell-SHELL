/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:42:44 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 17:55:25 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera la memoria usada por 'data', muestra un mensaje de error si hay,
	muestra el error de 'errno' si existe, y termina el programa con el
	código de salida indicado.
*/

int	exit_error(t_shell *data, const char *error, int exit_code, ...)
{
	va_list	args;

	if (data)
		clean_all(data);
	if (error)
	{
		va_start(args, exit_code);
		ft_var_printf_fd(STDERR, error, args);
		va_end(args);
	}
	if ((exit_code == FAILURE || exit_code == EXIT_CMD_NOT_EXECUTABLE) && errno)
		ft_printf_fd(STDERR, ERRNO, strerror(errno));
	exit(exit_code);
	return (exit_code);
}

int	exit_succes(t_shell *data, char *msg, int exit_code)
{
	if (data)
		clean_all(data);
	if (msg)
		ft_printf_fd(STDOUT, msg);
	exit(exit_code);
	return (exit_code);
}
