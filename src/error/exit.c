/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:42:44 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 03:26:18 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera la memoria usada por 'data', muestra un mensaje de error si hay,
	muestra el error de 'errno' si existe, y termina el programa con el
	código de salida indicado.
*/

int	exit_error(t_shell *data, char *error, int exit_code)
{
	if (data)
		clean_all(data);
	if (error)
		ft_printf_fd(STDERR, error);
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
