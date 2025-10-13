/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/12 19:25:17 by migarrid         ###   ########.fr       */
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
		ast_builder(&data, data.prompt.tokens, data.prompt.n_tokens);
		executor_recursive(&data, data.ast_root, FATHER);
		clean_cycle(&data, &data.prompt, &data.ast_root);
	}
	exit_succes(&data, MSG_GOODBYE, data.exit_code);
	return (data.exit_code);
}
