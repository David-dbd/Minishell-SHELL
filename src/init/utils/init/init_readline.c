/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:37:34 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:07:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 *	Asegura que el directorio en 'DEFAULT_HISTORY_FILE' exista.
 *	Si no está presente, lo crea con los permisos definidos
 *	para garantizar la disponibilidad de rutas de archivo.
*/

void	ensure_dir(const char *path)
{
	struct stat	st;

	st = (struct stat){0};
	if (stat(path, &st) == ERROR)
	{
		if (mkdir(path, DIR_PERMS) == ERROR)
			perror("minishell: mkdir:");
	}
}

/*
 * Configura readline interactivo: establece historial ilimitado,
 * habilita resaltado de sintaxis con colores personalizados y
 * configura multilínea para una mejor experiencia de usuario
 */

void	init_ic_readline(void)
{
	ensure_dir(DEFAULT_HYSTORY_DIR);
	open(DEFAULT_HISTORY_FILE, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMS);
	ic_set_history(DEFAULT_HISTORY_FILE, ILIMITED);
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
