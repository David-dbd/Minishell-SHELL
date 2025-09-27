/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/27 21:34:04 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Muestra el prompt "Minishell>> y guarda la entrada del usuario en `*input`
	Si `readline` falla y devuelve NULL:
	  - Si fue por Ctrl+D, `errno` no estará definido.
	  - Si fue por error del sistema, `errno` sí tendrá valor.
	En cualquier caso, libera la memoria de `data`.
	Devuelve el puntero al prompt introducido por el usuario.
*/

// char	*receive_input(char **input, t_shell *data)
// {
// 	*input = NULL;
// 	*input = readline("\033[1;34mminishell>\033[0m ");
// 	if (*input && input)
// 		add_history(*input);
// 	check_signals(data, input);
// 	return (*input);
// }

char	*receive_input(char **input, t_shell *data)
{
	*input = NULL;
	*input = ic_readline("\033[1;34mminishell\033[1;34m> \033[0m");
	if (*input && input)
		ic_history_add(*input);
	check_signals(data);
	return (*input);
}
