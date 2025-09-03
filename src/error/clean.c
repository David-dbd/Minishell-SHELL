/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:47:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 18:36:47 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	clean_tokens(t_prompt *prompt)
{
	int	i;

	i = 0;
	while (i < prompt->n_alloc_tokens)
	{
		if (prompt->tokens[i].value
			&& (prompt->tokens[i].type == WORD
				|| prompt->tokens[i].type == BUILT_IN
				|| prompt->tokens[i].type == COMMAND
				|| prompt->tokens[i].type == WILDCAR
				|| prompt->tokens[i].type == REDIR_APPEND
				|| prompt->tokens[i].type == REDIR_HEREDOC
				|| prompt->tokens[i].type == REDIR_INPUT
				|| prompt->tokens[i].type == REDIR_OUTPUT))
		{
			free(prompt->tokens[i].value);
			prompt->tokens[i].value = NULL;
		}
		i++;
	}
	free(prompt->tokens);
	prompt->tokens = NULL;
}

void	clean_prompt(t_prompt *prompt)
{
	if (prompt->prompt)
		free(prompt->prompt);
	clean_tokens(prompt);
	*prompt = (t_prompt){0};
}

void	clean_env(t_var *vars)
{
	t_var	*var;
	t_var	*next;

	var = vars;
	while (var)
	{
		next = var->next;
		if (var->key)
			free(var->key);
		if (var->value)
			free(var->value);
		free(var);
		var = next;
	}
}

void	clean_all(t_shell *data)
{
	clear_history();
	clean_prompt(&data->prompt);
	clean_env(data->env.vars);
}
