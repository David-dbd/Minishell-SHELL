/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:22:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/17 19:34:20 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	LÓGICA:
	En esta función se halla el old_len del str original: Hello $USER.
	Se haya el key_len de la key: $USER.
	Se haya el value_len del valor de la key (val_of_k): DAVID
	Luego, se operan los len para poder determinar el len del str reemplazando
	$USER -> DAVID y poder usarlo en ft_realloc, que hará la alocación.
	Luego se cuentan los '$' y se busca uno por uno, en busca de $USER.
	Se hace la cuenta para evitar reconocer falsos-positivos
	en los que str: Hello $ $USER. -> Se ignora el primer '$' de modo
	que no se haga ninguna operación desde ese ptr en memoria.
*/

int	copy_value(t_shell *d, char **token_val, char *key_value, char *key_to_find)
{
	char	*new_buffer;
	int		new_len;
	int		old_len;
	int		key_len;
	int		value_len;

	old_len = ft_strlen(*token_val);
	key_len = ft_strlen(key_to_find);
	value_len = ft_strlen(key_value);
	new_len = old_len - key_len - 1 + value_len;
	new_buffer = ft_realloc(*token_val, old_len + 1, new_len + 1);
	if (!new_buffer)
	{
		free (key_to_find);
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	}
	*token_val = new_buffer;
	new_buffer = ft_strchr(*token_val, '$');
	if (!new_buffer || !ft_isalpha(*(new_buffer + 1)))
		return (ERROR);
	ft_memmove(new_buffer + value_len, new_buffer + key_len + 1,
		ft_strlen(new_buffer + key_len + 1) + 1);
	ft_memcpy(new_buffer, key_value, value_len);
	return (SUCCESS);
}
