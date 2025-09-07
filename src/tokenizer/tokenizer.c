/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 20:20:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:03:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*g_type_names[] = {
	"START",
	"WORD",
	"GROUP",
	"PIPE",
	"REDIR_INPUT",
	"REDIR_OUTPUT",
	"REDIR_APPEND",
	"REDIR_HEREDOC",
	"COMMAND",
	"SCAPE",
	"BUILT_IN",
	"SEMICOLON",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"CMD_SUBSTITUTION",
	"PAREN_OPEN",
	"PAREN_CLOSE",
	"WILDCAR",
	"EXPANSION",
	"BACKGROUND",
	"AND",
	"OR",
	"EXPAN_VALUE",
	"EXPAN_CMD",
	"NO_SPACE",
};

static void	print_tokens_debug(t_prompt *prompt)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (prompt->tokens[i].value)
			printf("Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
}

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
	printf("Alloc Tokens: %d\n", data->prompt.n_alloc_tokens);
	printf("Syntax Tokens: %d\n", data->prompt.n_tokens);
}

/*
	Revisa cada token del input y valida operadores como '|', '(', ')',
	'&&' y '||'. Comprueba que estén correctamente colocados y emparejados.
*/

int	valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	logic_trans_args_cmd(data, tokens);
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
			|| (!check_cmd_syntax(data, prompt, tokens, i)))
			return (SYNTAX_ERROR);
		i++;
	}
	if (!check_parent_balance(data, prompt, tokens))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

int	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	allocate_tokens(data, prompt, input);
	get_tokens(data, prompt, input);
	// print_tokens_debug(prompt);
	if (!valid_tokens(data, prompt, prompt->tokens))
		return (SYNTAX_ERROR);
	// printf("------------------------------------------------\n");
	print_tokens_debug(prompt);
	simplify_tokens(data, prompt, prompt->tokens);
	// printf("------------------------------------------------\n");
	// print_tokens_debug(prompt);
	return (SUCCESS);
}
