/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:11:27 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 * Lee entrada del usuario con readline personalizado: almacena
 * en historial, verifica señales y retorna NULL con Ctrl+D
 * terminando el programa al finalizar el while y saliendo del programa
 */

char	*receive_input(char **input, t_shell *data)
{
	*input = NULL;
	*input = ic_readline("\033[1;34mminishell\033[1;34m> \033[0m");
	if (*input && input)
		ic_history_add(*input);
	check_signals(data);
	return (*input);
}

// char	*receive_input(char **input, t_shell *data)
// {
// 	*input = NULL;
// 	*input = readline("\033[1;34mminishell>\033[0m ");
// 	if (*input && input)
// 		add_history(*input);
// 	check_signals(data, input);
// 	return (*input);
// }
