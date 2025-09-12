/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cmd_to_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:39:12 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/12 16:51:29 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Decide si un token de tipo comando debe transformarse en WORD,
	para que los argumentos pegados a comandos se interpreten correctamente.
	Esto ocurre cuando:
	- El comando está al inicio (i == 1) y el anterior es un comando válido.
	- El token anterior es comando y no está precedido por redirección.
	- El token anterior es una comilla y antes había un comando.
	- Casos más complejos con comillas dobles/NO_SPACE antes de un comando.
*/

static int	before_is_cmd_word_redir(t_token *tokens, int i)
{
	if ((i >= 4 && tokens[i - 1].type == WORD
			&& tokens[i - 2].type == NO_SPACE
			&& tokens[i - 3].type == EXPANSION
			&& is_redir_type(tokens[i - 4].type)))
			return (0);
	if (i >= 2 && tokens[i - 1].type == WORD
			&& !is_redir_type(tokens[i - 2].type))
			return (1);
	else if (i >= 2 && tokens[i - 1].type == EXPANSION
			&& !is_redir_type(tokens[i - 2].type))
			return (1);
	else if (i >= 1 && is_cmd_builtin_type(tokens[i - 1].type))
		return (1);
	else if (i >= 1 && is_redir_type(tokens[i - 1].type))
		return (1);
	else if (i == 1 && tokens[i - 1].type == EXPANSION)
		return (1);
	return (0);
}

/*
	Revisa todos los tokens del prompt y ajusta tipos:
	- Si cumple condiciones de transformación, convierte COMMAND en WORD.
	- Si hay NO_SPACE cerca, también convierte el token en WORD.
	Esto asegura que los argumentos de comandos se tokenicen correctamente.
*/

void	transform_cmd_to_word(t_shell *data, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (i == 0 || i >=1 && is_delimiter_type(tokens[i - 1].type))
		{
			if (tokens[i].type == WORD)
				tokens[i].type = COMMAND;
		}
		else if (before_is_cmd_word_redir(tokens, i))
		{
			if (is_cmd_type(tokens[i].type))
				tokens[i].type = WORD;
		}
		i++;
	}
}
