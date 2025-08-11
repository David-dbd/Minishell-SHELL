/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_and.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:44:05 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:07:50 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_and(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str, "&&", 2))
	{
		add_token(tokens, "&&", AND);
		(*i)++;
	}
}
