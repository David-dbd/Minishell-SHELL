/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/14 00:43:54 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Función principal `main` de Minishell. Inicializa la estructura `data`
	con `init_minishell` y entra en el bucle principal. Lee la entrada del
	usuario, tokeniza, construye el AST y ejecuta los comandos. Limpia la
	memoria tras cada ciclo y termina con `exit_succes` al salir.
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
