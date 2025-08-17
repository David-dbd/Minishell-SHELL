/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   is_dolar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:58:46 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/16 15:52:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Si el carácter es '$', añade un token de tipo EXPANSION.
	Luego procesa el resto como palabra con is_word.
*/

static int	ft_is_expan(int c)
{
	if (ft_isalnum(c) || c == '_' || c == '?' || c == '$' || c == '!'
		|| c == '#' || c == '*' || c == '@')
		return (1);
	return (0);
}

void	is_dolar(t_shell *data, t_token *tokens, const char *str, int *i)
{
	int		len;
	int		start;
	char	*expansion;

	if (str[*i] == '$')
	{
		start = *i;
		(*i)++;
		while (str[*i] != '\0' && !ft_isspace(str[*i]) && ft_is_expan(str[*i]))
			(*i)++;
		len = *i - start;
		if (len > 0)
		{
			expansion = ft_substr(str, start, len);
			if (!expansion)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			add_token(tokens, expansion, EXPANSION);
		}
	}
}
