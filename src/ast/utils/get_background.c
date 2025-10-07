/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_background.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 20:47:36 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 21:45:40 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	get_background(t_token *tokens, int n_tokens, int *i)
{
	if (*i < n_tokens && tokens[*i].type == BACKGROUND)
	{
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}
