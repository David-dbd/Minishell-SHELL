/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:49:40 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 01:25:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Función para trnasformar tokens WORD en WILDCARDS en el caso específico
	en el que el valor del token tenga '*' al final.

	Ejemplo:

	-"hola*" ->Se cataloga como WORD pero esta mal. Debería de ser WILDCARD.

	Por esto se revisa el char final de su value.
*/

void	transform_word_to_wildcard(t_shell *d, t_prompt *promp, t_token *tokens)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (i < promp->n_tokens)
	{
		if (tokens[i].type == WORD)
		{
			if (tokens[i].value)
			{
				len = ft_strlen(tokens[i].value);
				if (len > 0 && tokens[i].value[len - 1] == '*')
					tokens[i].type = WILDCARD;
			}
		}
		i++;
	}
}
