/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:26:20 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 12:34:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Se llama al detectar un error de sintaxis: limpia el 'prompt' si tiene
	tokens, muestra el mensaje de error y guarda el código en
	'exit_code' sin terminar el programa.
*/

int	syntax_error(t_shell *data, const char *error, int exit_code, ...)
{
	va_list	args;

	if (error)
	{
		va_start(args, exit_code);
		ft_var_printf_fd(STDERR, error, args);
		va_end(args);
	}
	if (data)
	{
		clean_prompt(&data->prompt);
		data->exit_code = exit_code;
	}
	return (exit_code);
}

t_node	*error_status_node(t_shell *data, t_node **node, char *msg)
{
	if (data)
		data->error_state = TRUE;
	if (msg)
		ft_printf_fd(STDERR, msg);
	if (node)
		clean_node(node);
	return (NULL);
}

t_redir	*error_status_redirs(t_shell *data, t_redir **lst, char *msg)
{
	if (data)
		data->error_state = TRUE;
	if (msg)
		ft_printf_fd(STDERR, msg);
	if (lst)
		clean_redirs(lst);
	return (NULL);
}
