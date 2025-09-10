/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_tokens_for_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:57:13 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 21:57:25 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	send_tokens_for_expansion(t_shell *data, t_token *tokens, int phase)
{
	int	i;

	i = 0;
	while (i < data->prompt.n_alloc_tokens)
	{
		if (tokens[i].type == EXPANSION)
		{
			if (expansion(data, &tokens[i], &data->env, phase) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
