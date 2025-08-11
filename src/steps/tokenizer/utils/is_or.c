/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_or.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:08:02 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_or(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str, "||", 2))
	{
		add_token(tokens, "||", OR);
		(*i)++;
	}
}
