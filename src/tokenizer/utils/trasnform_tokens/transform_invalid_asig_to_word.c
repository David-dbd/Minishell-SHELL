/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_invalid_asig_to_word.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:56:00 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 01:09:31 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Si es una asignación pero el token (i - 1) es un word, puede que esto
	indique que la asinación esta mal catalogada, ya que se iteramos en el loop
	hacia atras siempre y cuando haya un WORD, por ejemplo: cmd/built-in ls a=1
	Veremos que cuando salgamos del loop, existe la posibilidad de que
	nos encontraremos con un cmd o un built-in, de modo que efectivamente la
	asignación no deberia ser ASIGNATION sino WORD. A menos de que el built-in
	en cuestion sea export.
*/

static void	aux_if_word(t_prompt *prompt, t_token *tokens, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && j < prompt->n_tokens && tokens[j].type == WORD)
		j--;
	if (j >= 0 && (tokens[j].type == COMMAND || (tokens[j].type == BUILT_IN
			&& ft_strcmp(tokens[j].value, BUILTIN_EXPORT) != 0)))
		tokens[i].type = WORD;
}

void	transform_invalid_asig_to_word(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (i >= 1 && tokens[i - 1].type)
		{
			if (tokens[i].type == ASIGNATION)
			{
				if (tokens[i - 1].type == COMMAND
					|| (tokens[i - 1].type == BUILT_IN
						&& ft_strcmp(tokens[i - 1].value, BUILTIN_EXPORT) != 0))
					tokens[i].type = WORD;
				else if (tokens[i - 1].type == WORD)
					aux_if_word(prompt, tokens, i);
			}
		}
		i++;
	}
}
