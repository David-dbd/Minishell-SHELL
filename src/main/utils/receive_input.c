/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:42:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 02:18:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	check_global_balance(t_prompt *prompt, t_token *tokens)
{
	int unbalance;
	int	global_balance;

	global_balance = NONE;
	unbalance = get_or_and_balance(prompt, tokens);
	if (unbalance == CANT_CONTINUE)
		return (CANT_CONTINUE);
	if (unbalance == KEEP_TRYING)
		return (KEEP_TRYING);
	global_balance = CMD_BALANCE;
	unbalance = get_paren_balance(prompt, tokens);
	if (unbalance < 0)
		return (CANT_CONTINUE);
	if (unbalance > 0)
		return (KEEP_TRYING);
	global_balance = CMD_BALANCE + PAREN_BALANCE;
	if (get_single_quotes_balance(prompt, tokens) != BALANCE
		|| get_double_quotes_balance(prompt, tokens) != BALANCE)
		return (KEEP_TRYING);
	global_balance = CMD_BALANCE + PAREN_BALANCE + QUOTE_BALANCE;
	if (global_balance == GLOBAL_BALANCE)
		return (BALANCE);
}

int	join_lines_until_balanced(t_shell *data, t_prompt *prompt, char **full_line)
{
	char	*line;
	char	*tmp;
	int		unbalance;

	while(42)
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

static char	*read_until_balanced(t_shell *data, char *initial_line)
{
	char	*trimmed_full_line;
	char	*full_line;

	full_line = ft_strdup(initial_line);
	if (!full_line)
		return(free(initial_line), NULL);
	if (join_lines_until_balanced(data, &data->prompt, &full_line) != OK)
		return (free(full_line), initial_line);
	return (free(initial_line), full_line);
}

/*
 * Lee entrada del usuario con readline personalizado: almacena
 * en historial, verifica señales y retorna NULL con Ctrl+D
 * terminando el programa al finalizar el while y saliendo del programa
 */

char	*receive_input(t_shell *data, t_prompt *prompt)
{
	char	*line;

	line = NULL;
	if (isatty(fileno(stdin)))
	{
		line = ic_readline("\033[1;34mminishell\033[1;34m> \033[0m");
		prompt->input = read_until_balanced(data, line);
		// prompt->input = line;
		if (prompt->input)
			ic_history_add(prompt->input);
	}
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		prompt->input = ft_strtrim(line, "\n");
		free(line);
	}
	check_signals(data, NULL, NULL, NULL);
	return (prompt->input);
}
