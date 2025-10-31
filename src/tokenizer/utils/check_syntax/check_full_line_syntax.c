/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_full_line_syntax.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:35:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 18:32:06 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	get_paren_balance(t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	balance;

	i = 0;
	balance = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == PAREN_OPEN)
			balance++;
		else if (tokens[i].type == PAREN_CLOSE)
			balance--;
		i++;
	}
	return (balance);
}

int	get_double_quotes_balance(t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	counter;
	int balance;

	i = 0;
	counter = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == DOUBLE_QUOTE)
			counter++;
		i++;
	}
	balance = counter % 2;
	return (balance);
}

int	get_single_quotes_balance(t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	counter;
	int balance;

	i = 0;
	counter = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == SINGLE_QUOTE)
			counter++;
		i++;
	}
	balance = counter % 2;
	return (balance);
}

int	get_or_and_balance(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == OR || tokens[i].type == AND)
		{
			if (i == 0)
				return (CANT_CONTINUE);
			if (i > 0 && tokens[i - 1].type && tokens[i + 1].type == NONE)
				return (KEEP_TRYING);
		}
		i++;
	}
	return (BALANCE);
}
