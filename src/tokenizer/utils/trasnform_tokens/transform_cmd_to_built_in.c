/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cmd_to_built_in.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:36:08 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 01:10:44 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	En casos edge como 'expo'rt - 'e'cho - c'd' - etc...
	Estos built-ins quedan catalogaods como COMMAND, lo cual esta mal.
	Esta función verifica si ese es el caso y los transforma a BUILT_IN
*/

void	transform_cmd_to_built_in(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == COMMAND)
		{
			if (is_built_in(&tokens[i], tokens[i].value))
				tokens[i].type = BUILT_IN;
		}
		i++;
	}
}
