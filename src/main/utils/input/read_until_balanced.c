/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_until_balanced.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 22:05:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/06 22:54:57 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	check_sub_balance(t_prompt *p, t_token *t, int (*f)(t_prompt *, t_token *))
{
	int	unbalance;

	unbalance = f(p, t);
	if (unbalance == CANT_CONTINUE)
		return (CANT_CONTINUE);
	if (unbalance == KEEP_TRYING)
		return (KEEP_TRYING);
	return (BALANCE);
}

int	check_global_balance(t_prompt *prompt, t_token *tokens)
{
	int	global_balance;

	global_balance = check_sub_balance(prompt, tokens, get_or_and_balance);
	if (global_balance != BALANCE)
		return (KEEP_TRYING);
	global_balance = check_sub_balance(prompt, tokens, get_pipe_balance);
	if (global_balance != BALANCE)
		return (KEEP_TRYING);
	global_balance = get_paren_balance(prompt, tokens);
	if (global_balance < 0)
		return (CANT_CONTINUE);
	if (global_balance > 0)
		return (KEEP_TRYING);
	if (get_single_quotes_balance(prompt, tokens) != BALANCE
		|| get_double_quotes_balance(prompt, tokens) != BALANCE)
		return (KEEP_TRYING);
	return (BALANCE);
}

int	join_lines_until_balanced(t_shell *data, t_prompt *prompt, char **full_line)
{
	char	*line;
	char	*tmp;
	int		unbalance;

	while (42)
	{
		allocate_tokens(data, prompt);
		get_tokens(data, prompt, *full_line);
		unbalance = check_global_balance(prompt, prompt->tokens);
		clean_prompt(prompt);
		if (unbalance == CANT_CONTINUE)
			return (CANT_CONTINUE);
		else if (unbalance == BALANCE)
			break ;
		line = ic_readline("> ");
		if (!line)
			break ;
		tmp = *full_line;
		*full_line = ft_strjoin_multi(3, tmp, " ", line);
		if (!(*full_line))
			return (free(tmp), free(line), exit_error(data, ERR_MALLOC, 1));
		free(tmp);
		free(line);
	}
	return (OK);
}

 char	*read_until_balanced(t_shell *data, char *initial_line)
{
	char	*full_line;

	full_line = ft_strdup(initial_line);
	if (!full_line)
		return (free(initial_line), NULL);
	if (join_lines_until_balanced(data, &data->prompt, &full_line) != OK)
		return (free(full_line), initial_line);
	return (free(initial_line), full_line);
}
