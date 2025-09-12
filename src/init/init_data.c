/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/12 17:28:54 by davdiaz-         ###   ########.fr       */
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
	data->extra_features.session_start = time(NULL);
	init_enviroment(data, envp);
	print_session_start(data, data->extra_features.session_start,
			data->extra_features.user_name);
}
