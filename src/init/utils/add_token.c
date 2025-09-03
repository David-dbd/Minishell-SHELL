/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:07:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 18:35:41 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Añade un nuevo token al array `tokens` con el valor y tipo dados.
	Si es de tipo EXPANSION, marca su flag `expand` como TRUE.
	Usa un índice estático para añadir en la posición correcta.
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

int	add_token(t_shell *data, t_prompt *prompt, char *value, int type)
{
	int		i;
	t_token	*tokens;

	i = prompt->n_tokens;
	check_buffer(data, prompt);
	tokens = prompt->tokens;
	tokens[i].id = i;
	tokens[i].value = value;
	tokens[i].type = type;
	if (tokens[i].type == EXPANSION)
		tokens[i].expand = TRUE;
	if (i > 0 && tokens[i - 1].type == DOUBLE_QUOTE)
		tokens[i].double_quoted = TRUE;
	if (i > 0 && tokens[i - 1].type == SINGLE_QUOTE)
		tokens[i].single_quoted = TRUE;
	prompt->n_tokens++;
	return (tokens[tokens[i].id].id);
}
