/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:42:44 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/13 19:56:28 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera la memoria usada por 'data', muestra un mensaje de error si lo hay,
	imprime el error de 'errno' si corresponde y finaliza el programa con el
	código de salida dado.
*/

int	exit_error(t_shell *data, const char *error, int exit_code, ...)
{
	va_list	args;

	if (error)
	{
		va_start(args, exit_code);
		ft_var_printf_fd(STDERR, error, args);
		va_end(args);
	}
	if ((exit_code == EXIT_FAIL) && errno && error != NULL)
		ft_printf_fd(STDERR, ERRNO, strerror(errno));
	if (data)
		clean_all(data);
	exit(exit_code);
	return (exit_code);
}

/*
	Imprime fin de sesión, libera la memoria de 'data', muestra 'msg' si existe
	y finaliza el programa con el código de salida dado.
*/

int	exit_succes(t_shell *data, char *msg, int exit_code)
{
	// if (msg)
	// {
	// 	print_session_end(data->extra_features.session_start, NULL);
	// 	if (data->extra_features.user_name)
	// 		ft_printf_fd(STDOUT, msg, data->extra_features.user_name);
	// 	else
	// 		ft_printf_fd(STDOUT, MSG_GOODBYE_V2);
	// }
	if (data)
		clean_all(data);
	exit(exit_code);
	return (exit_code);
}
