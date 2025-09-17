/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_invalid_asig_to_word.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:56:00 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/17 19:14:22 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	aux_if_word(t_prompt *prompt, t_token *tokens, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && j < prompt->n_tokens && tokens[j].type == WORD)
		j--;
	if (j >= 0 && tokens[j].type == COMMAND || tokens[j].type == BUILT_IN)
		tokens[i].type = WORD;
}

void	transform_invalid_asig_to_word(t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	j;

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
