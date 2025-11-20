/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:49:40 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 00:00:36 by migarrid         ###   ########.fr       */
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

static int	is_valid_wildcard(t_token *tokens, t_prompt *prompt, int i)
{
	int	j;

	j = i - 1;
	while (j >= 0 && is_arg_type(tokens[j].type))
		j--;
	if (j < 0)
		return (TRUE);
	if (is_delimiter_type(tokens[j].type) || is_redir_type(tokens[j].type)) //podria no haber cmds
		return (TRUE);
	if (tokens[j].type == BUILT_IN
		&& ft_strcmp(tokens[j].value, BUILTIN_EXPORT) == 0)
		return (FALSE);
	return (TRUE);
}

void	transform_word_to_wildcard(t_shell *d, t_prompt *promp, t_token *tokens)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (i < promp->n_tokens)
	{
		if ((tokens[i].type == WORD || tokens[i].type == FILENAME)
			&& !tokens[i].single_quoted && !tokens[i].double_quoted
			&& has_a_wildcard(tokens[i].value)
			&& is_valid_wildcard(tokens, promp, i))
		{
			tokens[i].type = WILDCARD;
		}
		i++;
	}
}
