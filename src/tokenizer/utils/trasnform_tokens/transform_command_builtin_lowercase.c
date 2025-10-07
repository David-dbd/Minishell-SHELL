/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_command_builtin_lowercase.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:40:49 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 16:08:19 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	normalize_token_to_lower(t_token *token)
{
	int		i;

	i = 0;
	while(token->value[i])
	{
		token->value[i] = ft_tolower(token->value[i]);
		i++;
	}
}

void	transform_command_built_lowercase(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while  (i < prompt->n_tokens)
	{
		if (is_cmd_builtin_type(tokens[i].type))
			normalize_token_to_lower(&tokens[i]);
		i++;
	}
}
