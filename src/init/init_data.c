/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 20:49:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Inicializa la estructura 'data' del shell: limpia valores,
	inicia tokens, AST y variables de entorno, guarda el inicio
	de sesión y muestra mensaje de bienvenida.
*/

void	init_data(t_shell *data, char **input, char **envp)
{
	*input = NULL;
	*data = (t_shell){0};
	data->prompt.tokens = NULL;
	data->ast_root = NULL;
	data->env.vars = NULL;
	data->session_start = time(NULL);
	init_enviroment(data, envp);
	print_session_start(data->session_start);
}
