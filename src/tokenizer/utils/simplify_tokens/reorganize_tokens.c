/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorganize_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 22:44:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 17:30:43 by davdiaz-         ###   ########.fr       */
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
}
