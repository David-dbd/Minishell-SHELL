/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_empty_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:57:46 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 09:24:55 by davdiaz-         ###   ########.fr       */
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
	if (!*new_str)
	{
		free(*str);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
}

/*
	Recorro el str y copio cada caracter. Si me encuentro el $ y lo siguiente
	es valido, ignoro todo eso y solo escribo un espacio. Si llegase
	a encontrar otro "$" después del que ya reemplaze por espacio, no pasa nada,
	ya que no afectara debido a que i != j, de modo que dejara los otros "$"
	sin afectar. Para ser mas exactos: EL primer "$" valido que encuentra
	lo reemplaza por escpacio e ignora el resto de "$".
*/

static void	ignore_words(t_shell *data, t_token *token, char **str, int len)
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
			if (token->double_quoted)
			{// en lugar de new_str[j++] = (*str)[i++]
			//porque(*str)[i++]puede ser cualquier otro simbolo que no sea alnum
				new_str[j++] = ' ';
				//i++;
			}
		}
		else
			new_str[j++] = (*str)[i++];
	}
	free(*str);
	*str = new_str;
}

int	expand_empty_str(t_shell *data, t_token *token, char **key_to_find)
{
	int	key_len;
	int	token_len;

	key_len = ft_strlen(*key_to_find) + 1; //+ 1 por el '$' que no se copio
	token_len = ft_strlen(token->value); //si su len es igual es porque
	if (token_len == key_len) // no hay nada mas
	{
		if (token->double_quoted)
		{
			ft_memset(token->value, 0, token_len);
			token->value[0] = 32;
			token->value[1] = '\0';
			return (REPLACED);
		}
		else
			eliminate_token(&data->prompt, data->prompt.tokens, token->id);
	}
	else
	{
		ignore_words(data, token, &token->value, token_len);
		return (REPLACED);
	}
	return (SUCCESS);
}
