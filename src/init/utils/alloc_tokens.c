/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:50:56 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 20:51:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Inicializa la estructura de tokens para el prompt, reserva memoria
	para un número inicial de tokens y termina si falla la memoria.
*/

void	allocate_tokens(t_shell *data, t_prompt *prompt, char *input)
{
	prompt->prompt = input;
	prompt->n_alloc_tokens = INIT_TOKENS;
	prompt->tokens = ft_calloc(prompt->n_alloc_tokens, sizeof(t_token));
	if (!prompt->tokens)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}
