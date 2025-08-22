/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:27 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/22 03:09:47 by migarrid         ###   ########.fr       */
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
	"OR"
};

static void	print_tokens_debug(t_prompt *prompt)
{
	int	i;

	i = 0;
	while (i < prompt->n_alloc_tokens)
	{
		if (prompt->tokens[i].value)
			printf("Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
}

/*
	Añade un nuevo token al array `tokens` con el valor y tipo dados.
	Si es de tipo EXPANSION, marca su flag `expand` como TRUE.
	Usa un índice estático para añadir en la posición correcta.
*/

int	add_token(t_token *tokens, char *value, int type)
{
	static int	i = 0;

	if (type == RESET)
	{
		i = 0;
		return (0);
	}
	tokens[i].id = i;
	tokens[i].value = value;
	tokens[i].type = type;
	if (tokens[i].type == EXPANSION)
		tokens[i].expand = TRUE;
	i++;
	return (tokens[tokens[i - 1].id].id);
}

/*
	Recorre el string `input` y llama a las funciones `is_*` para
	detectar cada tipo de token. Avanza el índice según lo detectado.
*/

void	get_tokens(t_shell *data, t_token *tokens, char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		is_not_token(input, &i);
		is_and(tokens, input, &i);
		is_or(tokens, input, &i);
		is_pipe(tokens, input, &i);
		is_parenten(tokens, input, &i);
		is_semicolon(tokens, input, &i);
		is_heredoc(data, tokens, input, &i);
		is_redir(data, tokens, input, &i);
		is_scape(data, tokens, input, &i);
		is_quote(data, tokens, input, &i);
		is_wildcar(data, tokens, input, &i);
		is_dolar(data, tokens, input, &i);
		is_word(data, tokens, input, &i);
		is_hash(input, &i);
	}
	calculate_tokens(&data->prompt, tokens);
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
	while (tokens[i].type)
	{
		if (!check_open_parent(data, prompt, tokens, i)
			|| (!check_close_parent(data, prompt, tokens, i))
			|| (!check_pipe(data, prompt, tokens, i))
			|| (!check_or_and(data, prompt, tokens, i))
			|| (!check_redir_input(data, prompt, tokens, i))
			|| (!check_redir_output(data, prompt, tokens, i))
			|| (!check_double_balance(data, prompt, tokens, i))
			|| (!check_single_balance(data, prompt, tokens, i))
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
	reset_tokens();
	allocate_tokens(data, prompt, input);
	get_tokens(data, prompt->tokens, input);
	// print_tokens_debug(prompt);
	if (!valid_tokens(data, prompt, prompt->tokens))
		return (SYNTAX_ERROR);
	// printf("------------------------------------------------\n");
	print_tokens_debug(prompt);
	return (SUCCESS);
}
