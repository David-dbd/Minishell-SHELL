/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieve_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:26:05 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Muestra el prompt "Minishell>> y guarda la entrada del usuario en `*input`
	Si `readline` falla y devuelve NULL:
	  - Si fue por Ctrl+D, `errno` no estará definido.
	  - Si fue por error del sistema, `errno` sí tendrá valor.
	En cualquier caso, libera la memoria de `data`.
	Devuelve el puntero al prompt introducido por el usuario.
*/

char	*recieve_input(char **input, t_shell *data)
{
	*input = readline("\033[1;34mminishell>\033[0m ");
	if (*input == NULL)
		clean_all(data);
	else if (*input && input)
		add_history(*input);
	return (*input);
}
