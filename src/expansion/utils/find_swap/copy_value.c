/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:22:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/12 14:01:08 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*static char	*find_key_in_new_buffer(char *token_val)
{
	char	*ptr;
	int		len;

	len = ft_count_char(token_val, '$');
	while (len > 0)
	{
		ptr = ft_strchr(token_val, '$');
		if (ptr && *ptr + 1)
		{
			if (ft_isalpha(*(ptr + 1)))
				return (ptr);
		}
		len--;
	}
	return (ptr);
}*/

int copy_value(t_shell *d, char **token_val, char *key_value, char *key_to_find)
{
	char	*ptr;
	char	*new_buffer;
	int		new_len;
	int		old_len;
	int		key_len;
	int		value_len;

	old_len = ft_strlen(*token_val);
	key_len = ft_strlen(key_to_find);
	value_len = ft_strlen(key_value);
	new_len = old_len - key_len - 1 + value_len;
	new_buffer = ft_realloc(*token_val, old_len, new_len);
	if (!new_buffer)
		return (ERROR);
	*token_val = new_buffer;
	ptr = ft_strchr(*token_val, '$');
	if (!ptr || !ft_isalpha(*(ptr + 1)))
		return (ERROR);
	ft_memmove(ptr + value_len, ptr + key_len + 1,
		ft_strlen(ptr + key_len + 1) + 1);
	ft_memcpy(ptr, key_value, value_len);
	return (SUCCESS);
}
/*
int	copy_value(t_shell *d, char **token_val, char *key_value, char *key_to_find)
{
	char	*ptr;
	int		new_len;
	int		old_len;
	int		i;

	i = 0;
	new_len = calculate_total_length(d, *token_val, key_to_find);
	old_len = ft_strlen(*token_val);
	ft_realloc(*token_val, old_len, new_len);
	if (!*token_val)
		return (ERROR);
	ptr = find_key_in_new_buffer(*token_val);
	while (key_value[i] != '\0')
	{
		ptr[i + 1] = key_value[i];
		i++;
	}
	return (SUCCESS);
}*/
