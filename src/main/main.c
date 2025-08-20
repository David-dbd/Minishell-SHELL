/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:07:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/20 20:46:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Aquí inicializamos la estructura principal `data` con las listas y datos
	con los que trabajaremos. También inicializamos la estructura `sa`
	para el manejo de señales con `sigaction`. Luego definimos un bucle
	infinito donde llamamos a `recieve_input` (que usa `readline`).
	Guardamos la entrada en el historial de comandos y empezamos el proceso
	con `steps_manager`.
*/

int	main(int argc, char **argv, char **envp)
{
	t_shell	data;
	char	*input;
	time_t	session_start;

	session_start = time(NULL);
	print_session_start(session_start);
	init_signals();
	init_data(&data, &input);
	while (recieve_input(&input, &data) != NULL)
	{
		if (!tokenizer(&data, &data.prompt, input))
			continue ;
		ast_built(&data, data.prompt.tokens);
		execute_recursive(&data, data.ast_root, &data.executor);
		clean_prompt(&data.prompt);
	}
	print_session_end(session_start);
	exit_succes(&data, MSG_GOODBYE, EXIT_SUCCESS);
	return (0);
}
