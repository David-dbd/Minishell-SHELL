/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:27 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 18:04:13 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	allocate_tokens(data, prompt, input);
	get_tokens(prompt->tokens, input);
	valid_tokens(prompt->tokens);
}
