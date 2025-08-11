/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 16:54:47 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_tokens(t_token *first)
{
}

void	init_prompt(t_prompt *prompt, char **input)
{
	*prompt = (t_prompt){0};
	init_token(prompt->tokens);
}

void	init_data(t_shell *data, char **input)
{
	*input = NULL;
	*data = (t_shell){0};
	init_prompt(&data->prompt, input);
}
