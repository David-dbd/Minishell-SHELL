/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:07:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 23:40:54 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Verifica si hay espacio en el array de tokens y lo duplica usando realloc.
*/

void	check_buffer(t_shell *d, t_prompt *p)
{
	size_t	new_capacity;
	t_token	*new_tokens;

	if (p->n_tokens >= p->n_alloc_tokens)
	{
		new_capacity = p->n_alloc_tokens * 2;
		if (new_capacity > INT_MAX)
			exit_error(d, ERR_MAX_TOKENS, EXIT_FAILURE);
		new_tokens = ft_realloc(p->tokens,
				p->n_alloc_tokens * sizeof(t_token),
				new_capacity * sizeof(t_token));
		if (!new_tokens)
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		p->tokens = new_tokens;
		p->n_alloc_tokens = new_capacity;
	}
}

/*
	Añade un token al prompt: asigna valor, tipo y marca comillas o expansión,
	luego incrementa el contador y devuelve el ID del token añadido.
*/

int	add_token(t_shell *data, t_prompt *prompt, char *value, int type)
{
	int		i;
	t_token	*tokens;

	i = prompt->n_tokens;
	check_buffer(data, prompt);
	tokens = prompt->tokens;
	tokens[i].id = i;
	tokens[i].hash = i;
	tokens[i].value = value;
	tokens[i].type = type;
	prompt->n_tokens++;
	return (tokens[tokens[i].id].id);
}
