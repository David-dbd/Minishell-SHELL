/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/15 20:05:08 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Inicializa la estructura 'data' del shell: limpia valores,
	inicia tokens, AST y variables de entorno, guarda el inicio
	de sesión y muestra mensaje de bienvenida.
*/
void	init_ic_readline(void)
{
	ic_set_history(NO_FILE, ILIMITED);
	ic_set_prompt_marker("", "");
	ic_enable_multiline(true);
	ic_enable_multiline_indent(false);
	ic_enable_color(true);
	ic_style_def("ic-prompt", "#4A90E2");
	ic_style_def("ic-info", "#6272a4");
	ic_style_def("ic-diminish", "#6272a4 italic");
	ic_style_def("ic-emphasis", "#8be9fd bold");
	ic_style_def("ic-hint", "#6272a4");
	ic_style_def("ic-error", "#ff5555 bold");
	ic_style_def("ic-bracematch", "#50fa7b bold");
}

void	init_data(t_shell *data, char **input, char **envp)
{
	*input = NULL;
	*data = (t_shell){0};
	data->prompt.tokens = NULL;
	data->ast_root = NULL;
	data->env.vars = NULL;
	data->extra_features.session_start = time(NULL);
	init_ic_readline();
	init_enviroment(data, envp);
	print_session_start(data, data->extra_features.session_start,
		data->extra_features.user_name);
}
