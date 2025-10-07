/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:10 by migarrid          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/10/06 21:28:39 by davdiaz-         ###   ########.fr       */
=======
/*   Updated: 2025/10/06 19:36:56 by migarrid         ###   ########.fr       */
>>>>>>> mikel
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
	"ASIGNATION",
	"BACKGROUND",
	"AND",
	"OR",
	"EXPAN_VALUE",
	"EXPAN_CMD",
	"NO_SPACE",
	"LOCAL",
	"ENV",
	"DELETE",
	"EXP",
	"INDIFERENT",
	"PLUS_ASIGNATION",
	"TEMP_ASIGNATION",
	"FILENAME",
	"SUBSHELL",
	"SHELL"
};

static void	print_tokens_debug(t_prompt *prompt)
{
	int	i;

	i = 0;
	printf("------------------------------------------------\n");
	while (i < prompt->n_tokens)
	{
		if (prompt->tokens[i].value)
			printf("Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
}

void	test_built_in(t_shell *data, t_token *tokens, int n_tokens)
{
	int	i;

	i = 0;
	while (i < n_tokens)
	{
		if (tokens[i].type == BUILT_IN || is_asignation_type(tokens[i].type))
			which_builtin(data, tokens, &tokens[i]);
		i++; //ESTO HACE QUE SE MULTIPLIQUEN LOS RESULTADOS x2
	}
}

/*
	Recorre el string `input` y llama a las funciones `is_*` para
	detectar cada tipo de token. Avanza el índice según lo detectado.
*/

void	parse_tokens(t_shell *data, t_prompt *prompt, char *input)
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

int	check_if_valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
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
	if (!check_parent_balance(data, prompt, tokens)
		|| (!check_double_parent(data, tokens, prompt)))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}

int	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	allocate_tokens(data, prompt, input);
	parse_tokens(data, prompt, input);
	if (!check_if_valid_tokens(data, prompt, prompt->tokens))
		return (SYNTAX_ERROR);

	print_tokens_debug(prompt);

	// is_it_quoted(prompt, prompt->tokens); // Se puede hacer mas eficiente

	expansion(data, prompt->tokens, &data->env, FINAL_PHASE);
	simplify_tokens(data, prompt, prompt->tokens);

	// print_tokens_debug(prompt);

	transform_tokens_logic(data, prompt, prompt->tokens);

	// print_tokens_debug(prompt);

	test_built_in(data, prompt->tokens, prompt->n_tokens);
	//send_tokens_for_asig(data, prompt->tokens, FINAL_PHASE);
	eliminate_temp_asig(prompt, prompt->tokens);

	print_tokens_debug(prompt);
	return (SUCCESS);
}
