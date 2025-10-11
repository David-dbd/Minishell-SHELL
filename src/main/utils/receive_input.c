/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 06:39:42 by migarrid         ###   ########.fr       */
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
	char	*line;

	*input = NULL;
	if (isatty(fileno(stdin)))
	{
		*input = ic_readline("\033[1;34mminishell\033[1;34m> \033[0m");
		if (*input && input)
			ic_history_add(*input);
	}
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		*input = ft_strtrim(line, "\n");
		free(line);
	}
	check_signals(data, NULL, NULL);
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
