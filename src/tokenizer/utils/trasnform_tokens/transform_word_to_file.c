/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_file.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:43:29 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:09:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Estas funciones reconocen tokens WORD que estan después de un REDIR
	y los reasigna como FILENAME.
	También verifican si el token que esta después de FILENAME es un COMMAND
	y si se debería cambiar por un WORD o dejarse basado en prompts como:

	* echo hola > file como estas? -> "hola como estas?" son args del echo
	* (echo hola) > file como estas? -> "como estas?" es un cmd con su arg

	Logica:
	Voy hacia atras ignorando FILENAME y REDIR. Si al acabar de iterar estoy en
	un word entonces el COMMAND debe cambiarse a WORD, pero si en otra cosa
	(concretamente un ")" ) entonces debe quedarse como COMMAND.
*/

static int	search_for_paren(t_token *tokens, int i)
{
	if (tokens[i].type == WORD || is_cmd_builtin_type(tokens[i].type))
		i--;
	while (i > 0 && (is_redir_type(tokens[i].type)
			|| tokens[i].type == FILENAME))
		i--;
	if (tokens[i].type == WORD)
		return (TRUE);
	else if (tokens[i].type == PAREN_CLOSE)
		return (FALSE);
	return (FALSE);
}

void	transform_word_to_file(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (i > 0 && tokens[i].type == WORD
			&& is_redir_type(tokens[i - 1].type))
		{
			tokens[i].type = FILENAME;
		}
		if (i > 0 && is_cmd_builtin_type(tokens[i].type)
			&& tokens[i - 1].type == FILENAME
			&& search_for_paren(tokens, i))
		{
			tokens[i].type = WORD;
		}
		else if (i > 0 && tokens[i].type == WORD
			&& tokens[i - 1].type == FILENAME && !search_for_paren(tokens, i))
		{
			tokens[i].type = COMMAND;
		}
		i++;
	}
}
