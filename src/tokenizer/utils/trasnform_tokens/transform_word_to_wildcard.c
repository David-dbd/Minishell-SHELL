/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:49:40 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/23 11:15:49 by davdiaz-         ###   ########.fr       */
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
	int	len;
	int	i;

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

static int	has_a_wildcard(char *value)
{
	int	len;
	int	result;

	if (!value)
		return (FALSE);
	len = ft_strlen(value);
	if (len == 0)
		return (FALSE);
	if (value[0] == '*')
		return (TRUE);
	if (value[len - 1] == '*')
		return (TRUE);
	result = has_middle_wildcard(value);
	return (result);
}

static int	is_valid_wildcard(t_token *tokens, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && is_arg_type(tokens[j].type))
		j--;
	if (j < 0)
		return (TRUE);
	if (is_delimiter_type(tokens[j].type) || is_redir_type(tokens[j].type))
		return (TRUE);
	return (TRUE);
}

void	transform_word_to_wildcard(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if ((tokens[i].type == WORD || tokens[i].type == FILENAME
				|| tokens[i].type == COMMAND)
			&& !tokens[i].single_quoted && !tokens[i].double_quoted
			&& has_a_wildcard(tokens[i].value)
			&& is_valid_wildcard(tokens, i))
		{
			tokens[i].type = WILDCARD;
		}
		i++;
	}
}
