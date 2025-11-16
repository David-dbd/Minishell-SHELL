/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:10 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/16 15:35:18 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Recorre el string `input` y llama a las funciones `is_*` para
	detectar cada tipo de token. Avanza el índice según lo detectado.
*/

void	get_tokens(t_shell *data, t_prompt *prompt, char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		is_not_token(input, &i);
		is_and(data, prompt, input, &i);
		is_or(data, prompt, input, &i);
		is_pipe(data, prompt, input, &i);
		is_parenten(data, prompt, input, &i);
		is_semicolon(data, prompt, input, &i);
		is_cmdsubs(data, prompt, input, &i);
		is_heredoc(data, prompt, input, &i);
		is_redir(data, prompt, input, &i);
		is_scape(data, prompt, input, &i);
		is_single_quote(data, prompt, input, &i);
		is_double_quote(data, prompt, input, &i);
		is_wildcar(data, prompt, input, &i);
		is_dolar(data, prompt, input, &i);
		is_word(data, prompt, input, &i);
		is_hash(input, &i);
	}
	// printf("Alloc Tokens: %d\n", data->prompt.n_alloc_tokens);
	// printf("Syntax Tokens: %d\n", data->prompt.n_tokens);
}

/*
	Revisa cada token del input y valida operadores como '|', '(', ')',
	'&&' y '||'. Comprueba que estén correctamente colocados y emparejados.
*/

int	check_if_valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	//print_tokens_debug(prompt);
	while (i < prompt->n_tokens)
	{
		if (!check_open_parent(data, prompt, tokens, i)
			|| (!check_close_parent(data, prompt, tokens, i))
			|| (!check_pipe(data, prompt, tokens, i))
			|| (!check_or_and(data, prompt, tokens, i))
			|| (!check_redir_input(data, prompt, tokens, i))
			|| (!check_redir_output(data, prompt, tokens, i))
			|| (!check_double_balance(data, prompt, tokens))
			|| (!check_single_balance(data, prompt, tokens))
			|| (!check_background(data, prompt, tokens, i))
			|| (!check_semicolon(data, prompt, tokens, i))
			|| (!check_cmd_syntax(data, prompt, tokens, i))
			|| (!check_cmd_externs(data, prompt, tokens, i)))
			return (SYNTAX_ERROR);
		i++;
	}
	if (!check_parent_balance(data, prompt, tokens))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

int	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	allocate_tokens(data, prompt);
	get_tokens(data, prompt, input);
	if (!check_if_valid_tokens(data, prompt, prompt->tokens))
		return (SYNTAX_ERROR);

	// printf("antes de expansion en la ejecución\n\n");
	// print_tokens_debug(prompt);

	// is_it_quoted(prompt, prompt->tokens); // Se puede hacer mas eficiente

	if (expansion(data, prompt->tokens, 0, INITIAL_PHASE) == ERROR)
		return (FAILURE);
	//printf("\n\nAFTER EXPANSION INITIAL_PHASE\n\n");
	//print_tokens_debug(prompt);

	simplify_tokens(data, prompt, prompt->tokens);

	transform_tokens_logic(data, prompt, prompt->tokens);

	simplify_tokens(data, prompt, prompt->tokens);

	//print_tokens_debug(prompt);
	//print_tokens_debug(prompt);

	// if (!cmd_correction(data, prompt->tokens, prompt->n_tokens))
	// 	return (FAILURE);
	// else
	// 	transform_tokens_logic(data, prompt, prompt->tokens);

	expand_wildcards(data, prompt, prompt->tokens, INITIAL_PHASE);

	//printf("after expand_wildcards\n\n");

	transform_tokens_logic(data, prompt, prompt->tokens);

	if (!check_if_valid_tokens(data, prompt, prompt->tokens))
		return (SYNTAX_ERROR);

	// print_tokens_debug(prompt);

	// printf("After TOKENIZER\n\n");
	return (SUCCESS);
}
