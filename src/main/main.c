/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 05:10:16 by migarrid         ###   ########.fr       */
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

	init_minishell(&data, argc, argv, envp);
	while (receive_input(&input, &data) != NULL)
	{
		if (!tokenizer(&data, &data.prompt, input))
			continue ;
		ast_built(&data, data.prompt.tokens);
		execute_recursive(&data, data.ast_root, &data.executor);
		clean_prompt(&data.prompt);
	}
	exit_succes(&data, MSG_GOODBYE, EXIT_SUCCESS);
	return (0);
}
