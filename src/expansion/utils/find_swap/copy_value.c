/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:22:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 21:23:14 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static char	*find_key_in_new_buffer(char *token_val, int len)
{
	char	*ptr;

	while (len > 0)
	{
		ptr = ft_strchr(token_val, '$');
		if (ptr && ft_isalpha(*(ptr + 1)))
			return (ptr);
		len--;
	}
	return (ptr);
}

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
	ptr = find_key_in_new_buffer(*token_val, new_len);
	while (key_value[i] != '\0')
	{
		ptr[i + 1] = key_value[i];
		i++;
	}
	return (SUCCESS);
}
