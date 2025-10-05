/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_word_to_file.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:43:29 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 20:52:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void transform_word_to_file(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (i > 0 && tokens[i].type == WORD && is_redir_type(tokens[i - 1].type))
			tokens[i].type = FILENAME;
		if (i > 0 && tokens[i].type == COMMAND && tokens[i - 1].type == FILENAME)
			tokens[i].type = WORD;
		i++;
	}
}
