/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:18:26 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/17 19:20:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	En esta función se verifica si la expansión a reemplazar viene sola o si
	viene acompañada de otros str.

	Eg. "Hello $USE" vs $USE

	Es por esto que se verifica el len de la key_to_find asi como del
	token->value; Si son iguales, entonces significa que el
	str solo contiene: $USE.
	Sin embargo, si por el contrario no son iguales, significa que el str
	puede contener mas cosas: "HELLO $USE".

	En este último caso, se necesita reconocer en dónde se ubica la clave a
	reemplazar sin afectar el resto de la str.
*/

static void	aux_alloc_mem(t_shell *data, char **str, char **new_str, int len)
{
	*new_str = ft_calloc(len + 1, sizeof(char));
	if (!new_str)
	{
		free(*str);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
}

static void	ignore_words(t_shell *data, char **str, int len)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	aux_alloc_mem(data, str, &new_str, len);
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] == '$' && (*str)[i + 1]
			&& ft_isalpha((*str)[i + 1]) && i == j)
		{
			i++;
			while (ft_isalnum((*str)[i]) || (*str)[i] == '_')
				i++;
			new_str[j++] = ' ';
		}
		else
			new_str[j++] = (*str)[i++];
	}
	free(*str);
	*str = new_str;
}

static void	expand_empty_str(t_shell *data, t_token *token, char **key_to_find)
{
	int		key_len;
	int		token_len;

	key_len = ft_strlen(*key_to_find) + 1; //+ 1 por el '$' que no se copio
	token_len = ft_strlen(token->value); //si su len es igual es porque
	if (token_len == key_len) // no hay nada mas
	{
		free (token->value);
		token->value = ft_strdup("");
		if (!token->value) // por ""y solo necesito que reemplaze la expansion
		{
			free (*key_to_find);
			exit_error(data, ERR_MALLOC, EXIT_USE);
		}
	}
	else
		ignore_words(data, &token->value, token_len);
}

//copy_value(data, &token->value, " ", *key_to_find);

int	extract_key(t_shell *data, t_token *token, char **key_to_find, int phase)
{
	int	found;
	int	match;

	found = FALSE;
	match = copy_key(token->value, key_to_find);
	if (!match)
		return (FAILURE);
	found = find_key_in_lst(data, token, key_to_find);
	if (found == ERROR)
		return (ERROR);
	if (!found && phase == FINAL_PHASE)
	{
		expand_empty_str(data, token, key_to_find);
		token->type = WORD;
	}
	return (SUCCESS);
}
