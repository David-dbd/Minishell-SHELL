/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:37:34 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/06 17:21:46 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Inicializa la librería de readline interactiva (ic_readline)
	configurando historial, marcador de prompt, multilinea, colores
	y estilos de texto para resaltar distintos elementos en la terminal.
*/

// Highlighter minimalista y elegante
static void highlighter(ic_highlight_env_t *henv, const char *input, void *arg)
{
	size_t len = strlen(input);
	size_t i = 0;

	while (i < len)
	{
		char c = input[i];

		// Comentarios (#)
		if (c == '#')
		{
			ic_highlight(henv, i, len - i, "comment");
			break;
		}

		// Strings dobles ("")
		if (c == '"')
		{
			size_t start = i++;
			while (i < len && input[i] != '"') {
				if (input[i] == '\\' && i + 1 < len)
					i += 2;
				else
					i++;
			}
			if (i < len) i++;
			ic_highlight(henv, start, i - start, "string");
			continue;
		}

		// Strings simples ('')
		if (c == '\'')
		{
			size_t start = i++;
			while (i < len && input[i] != '\'') {
				if (input[i] == '\\' && i + 1 < len)
					i += 2;
				else
					i++;
			}
			if (i < len) i++;
			ic_highlight(henv, start, i - start, "string-single");
			continue;
		}

		// Operador && (lógico AND)
		if (c == '&' && i + 1 < len && input[i + 1] == '&')
		{
			ic_highlight(henv, i, 2, "op-and");
			i += 2;
			continue;
		}

		// Operador || (lógico OR)
		if (c == '|' && i + 1 < len && input[i + 1] == '|')
		{
			ic_highlight(henv, i, 2, "op-or");
			i += 2;
			continue;
		}

		// Pipe simple |
		if (c == '|')
		{
			ic_highlight(henv, i, 1, "op-pipe");
			i++;
			continue;
		}

		// Background &
		if (c == '&')
		{
			ic_highlight(henv, i, 1, "op-background");
			i++;
			continue;
		}

		// Redirección >> o >
		if (c == '>')
		{
			if (i + 1 < len && input[i + 1] == '>') {
				ic_highlight(henv, i, 2, "op-redirect");
				i += 2;
			} else {
				ic_highlight(henv, i, 1, "op-redirect");
				i++;
			}
			continue;
		}

		// Redirección << o <
		if (c == '<')
		{
			if (i + 1 < len && input[i + 1] == '<') {
				ic_highlight(henv, i, 2, "op-redirect");
				i += 2;
			} else {
				ic_highlight(henv, i, 1, "op-redirect");
				i++;
			}
			continue;
		}

		// Separador ;
		if (c == ';')
		{
			ic_highlight(henv, i, 1, "op-separator");
			i++;
			continue;
		}

		// Paréntesis ()
		if (c == '(' || c == ')')
		{
			ic_highlight(henv, i, 1, "op-paren");
			i++;
			continue;
		}

		// Números
		if (isdigit(c))
		{
			size_t start = i;
			while (i < len && (isdigit(input[i]) || input[i] == '.'))
				i++;
			ic_highlight(henv, start, i - start, "number");
			continue;
		}
		i++;
	}
}

/*
 * Configura readline interactivo: establece historial ilimitado,
 * habilita resaltado de sintaxis con colores personalizados y
 * configura multilínea para una mejor experiencia de usuario
 */

void	init_ic_readline(void)
{
	ic_set_history(DEFAULT_HISTORY, ILIMITED);
	ic_set_prompt_marker("", "");
	ic_enable_multiline(true);
	ic_enable_multiline_indent(false);
	ic_enable_highlight(true);
	ic_enable_color(true);
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
