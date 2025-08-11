/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:42:44 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 16:18:52 by migarrid         ###   ########.fr       */
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
	if (errno)
		ft_printf_fd(STDERR, ERRNO, strerror(errno));
	exit(exit_code);
	return (exit_code);
}
