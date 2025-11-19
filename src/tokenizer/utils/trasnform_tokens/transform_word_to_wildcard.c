/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:49:40 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/19 01:43:36 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Función para trnasformar tokens WORD en WILDCARDS en el caso específico
	en el que el valor del token tenga '*' al final.

	Ejemplo:

	-"hola*" ->Se cataloga como WORD pero esta mal. Debería de ser WILDCARD.

	Por esto se revisa el char final de su value.

	También se ve el caso específico en que tenga el wildcard al incio:
	*tmp
	Aunque el tokenizador lo cataloga de manera correcta (wildcard), puede
	que si tiene un NO_SPACE delante y un WORD, va a unir el wildcard con el
	WORD, de modo que quedara un WORD (que es correcto según
	las transformaciones), sin embargo, en este caso hay que reconvertirlo a
	WILDCARD y revertir la desición de simplify_tokens y las transformaciones
*/

static int	has_middle_wildcard(char *value)
{
	int len;
	int i;

	if (!value)
		return (FALSE);
	len = ft_strlen(value);
	if (len <= 2)
		return (FALSE);
	i = 1;
	while (i < len - 1)
	{
		if (value[i] == '*')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static int	aux_if_word(t_prompt *prompt, t_token *tokens, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && j < prompt->n_tokens && tokens[j].type == WORD)
		j--;
	if (j >= 0 && (tokens[j].type == BUILT_IN
			&& ft_strcmp(tokens[j].value, BUILTIN_EXPORT) != 0))
	{
		tokens[i].type = WORD;
		return (1);
	}
	return (0);
}

void	transform_word_to_wildcard(t_shell *d, t_prompt *promp, t_token *tokens)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (i < promp->n_tokens)
	{
		if (tokens[i].type == WORD && aux_if_word(promp, tokens, i))
		{
			if (tokens[i].value)
			{
				len = ft_strlen(tokens[i].value);
				if (len > 0 && tokens[i].value[len - 1] == '*')
					tokens[i].type = WILDCARD;
				else if (tokens[i].value[0] == '*')
				{
					tokens[i].type = WILDCARD;
				}
				else if (has_middle_wildcard(tokens[i].value))
					tokens[i].type = WILDCARD;
			}
		}
		i++;
	}
}
