/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:17:31 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/19 18:30:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	calculate_tokens(t_prompt *prompt, t_token *tokens)
{
	int	i;

	while(tokens[i].value)
		i++;
	prompt->n_tokens = i;
}
