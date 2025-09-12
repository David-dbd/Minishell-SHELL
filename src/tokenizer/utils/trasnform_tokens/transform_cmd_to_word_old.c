/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cmd_to_word_old.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:39:12 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/11 17:37:02 by migarrid         ###   ########.fr       */
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

static int	should_tranform_token(t_token *tokens, int i)
{
	if ((i == 1 && is_cmd_type(tokens[i - 1].type))
		|| (i >= 2 && is_cmd_type(tokens[i - 1].type)
			&& !is_redir_type(tokens[i - 2].type))
		|| (i >= 2 && is_quote_type(tokens[i - 1].type)
			&& is_cmd_type(tokens[i - 2].type))
		|| (i >= 1 && tokens[i - 1].type == EXPANSION)
		|| (i >= 4 && is_quote_type(tokens[i - 1].type)
			&& is_quote_type(tokens[i - 2].type)
			&& tokens[i - 3].type == NO_SPACE
			&& is_cmd_type(tokens[i - 4].type)))
		return (1);
	return (0);
}

/*
	Verifica si algún token cercano antes de este es NO_SPACE.
	Si es así, el token actual puede ser un argumento pegado al comando.
*/

static int	should_handle_no_space(t_token *tokens, int i)
{
	if ((i >= 1 && tokens[i - 1].type == NO_SPACE)
		|| (i >= 2 && tokens[i - 2].type == NO_SPACE
			&& !is_delimiter_type(tokens[i - 1].type))
		|| (i >= 3 && tokens[i - 3].type == NO_SPACE
			&& !is_delimiter_type(tokens[i - 2].type)
			&& !is_delimiter_type(tokens[i - 1].type)))
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
		if (should_tranform_token(tokens, i))
		{
			if (is_quote_type(tokens[i].type))
			{
				if (is_cmd_type(tokens[i + 1].type))
					tokens[i + 1].type = WORD;
			}
			else if (is_cmd_type(tokens[i].type))
				tokens[i].type = WORD;
		}
		else if (should_handle_no_space(tokens, i))
		{
			if (is_cmd_type(tokens[i].type))
				tokens[i].type = WORD;
		}
		i++;
	}
}
