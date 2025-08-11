/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:56 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 18:02:09 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	allocate_tokens(t_shell *data, t_prompt *prompt, char *input)
{
	int	n_alloc_tokens;

	prompt->prompt = input;
	n_alloc_tokens = ft_strlen(input);
	prompt->tokens = ft_calloc(n_alloc_tokens, sizeof(t_token));
	if (!prompt->tokens)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}
