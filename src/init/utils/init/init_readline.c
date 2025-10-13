/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:37:34 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/13 19:08:31 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Configura readline interactivo: establece historial ilimitado,
 * habilita resaltado de sintaxis con colores personalizados y
 * configura multilínea para una mejor experiencia de usuario
 */

void	init_ic_readline(void)
{
	ic_set_history(DEFAULT_HISTORY, ILIMITED);
	ic_set_prompt_marker("", "");
	ic_enable_multiline_indent(false);
	ic_style_def("ic-prompt", "#4A90E2");
	ic_style_def("ic-info", "#6272a4");
	ic_style_def("ic-diminish", "#6272a4 italic");
	ic_style_def("ic-emphasis", "#8be9fd bold");
	ic_style_def("ic-hint", "#6272a4");
	ic_style_def("ic-error", "#ff5555 bold");
	ic_style_def("ic-bracematch", "#74a5eeff bold");
	ic_style_def("op-and", "#9a98a3");
	ic_style_def("op-or", "#8a889a");
	ic_style_def("op-pipe", "#a8a6b0");
	ic_style_def("op-redirect", "#9d9ba8");
	ic_style_def("op-background", "#938fa0");
	ic_style_def("op-separator", "#6f6d78");
	ic_style_def("op-paren", "#a3a1ad");
	ic_style_def("string", "#a5a3ae");
	ic_style_def("string-single", "#95939e");
	ic_style_def("number", "#b0aeba");
	ic_style_def("comment", "#605e68 italic");
	ic_set_default_highlighter(highlighter, NULL);
}
