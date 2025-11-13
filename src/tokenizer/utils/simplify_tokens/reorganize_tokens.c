/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorganize_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 22:44:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 01:04:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Inicializa los tokens "vacíos" al final del arreglo
	para que no queden datos residuales tras mover tokens.
*/

void	void_tokens_at_the_end(t_token *tokens, int n_alloc, int n_tokens)
{
	int	i;

	i = n_tokens;
	while (i < n_alloc)
	{
		tokens[i] = (t_token){0};
		i++;
	}
}

/*
	Liberar la memoria de los tokens en un rango específico
	si son tipos que reservan memoria (WORD, COMMAND, REDIR, etc.)
*/

static void	free_tokens_in_range(t_token *tokens, int *range)
{
	int	i;

	i = range[0];
	while (i <= range[1])
	{
		if (is_alloc_type(tokens[i].type))
		{
			if (tokens[i].value)
				free(tokens[i].value);
		}
		i++;
	}
}

/*
	Reorganiza los tokens después de unir un bloque:
	- Libera la memoria de los tokens antiguos en el rango.
	- Guarda el resultado unido en el primer token del rango.
	- Mueve los tokens siguientes hacia adelante para cubrir el hueco.
	- Ajusta el número de tokens syntacticos
	- inicializa a 0 los tokens nuevos del final
*/

void	reorganize_tokens(t_prompt *p, t_token *tokens, int *range, char *res)
{
	int	tokens_to_move;
	int	tokens_to_remove;

	tokens_to_remove = range[1] - range[0];
	tokens_to_move = p->n_alloc_tokens - range[1] - 1;
	free_tokens_in_range(tokens, range);
	tokens[range[0]].value = res;
	tokens[range[0]].type = WORD;
	ft_memmove(&tokens[range[0] + 1],
		&tokens[range[1] + 1],
		(tokens_to_move * sizeof(t_token)));
	p->n_tokens = p->n_tokens - tokens_to_remove;
	void_tokens_at_the_end(tokens, p->n_alloc_tokens, p->n_tokens);
	adjust_id(tokens, p->n_tokens);
}

/*
	Función para eliminar tokens NO_SPACE cuando estan al lado de un delimitador
	Por ejemplo:
	*$PW && ls -> * - no_space - expansion - && -> * - no_space - &&
*/

int	no_space_at_delimiter(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == NO_SPACE && (i + 1) < prompt->n_tokens
			&& is_delimiter_type(tokens[i + 1].type))
		{
			// printf("no_space_at_delimiter: eliminate\n\n");
			eliminate_token(prompt, tokens, i);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

/*
	Función para eliminar tokens NO_SPACE manualmente si llegan a estar en el
	último indice. Esto puede pasar por ejemplo con:
	echo *$PW -> echo - * - no_space - expansion -> echo - * - no_space
*/

int	no_space_at_end(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == NO_SPACE && (i + 1) == prompt->n_tokens)//si es el ultimo token
		{
			// printf("no_scape_at_end: eliminate\n\n");
			eliminate_token(prompt, tokens, i);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
