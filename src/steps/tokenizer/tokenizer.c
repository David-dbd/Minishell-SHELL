/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:27 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/16 23:50:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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
		is_heredoc(tokens, input, &i);
		is_and(tokens, input, &i);
		is_or(tokens, input, &i);
		is_redir(tokens, input, &i);
		is_pipe(tokens, input, &i);
		is_parenten(tokens, input, &i);
		is_quote(tokens, input, &i);
		is_semicolon(tokens, input, &i);
		is_scape(tokens, input, &i);
		is_wildcar(tokens, input, &i);
		is_dolar(data, tokens, input, &i);
		is_word(data, tokens, input, &i);
		is_hash(input, &i);
	}
}

/*
	Revisa cada token del input y valida operadores como '|', '(', ')',
	'&&' y '||'. Comprueba que estén correctamente colocados y emparejados.
*/

int	valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type && prompt->error == VIRGIN)
	{
		check_open_parent(data, prompt, tokens, i);
		check_close_parent(data, prompt, tokens, i);
		check_pipe(data, prompt, tokens, i);
		check_or_and(data, prompt, tokens, i);
		check_redir_input(data, prompt, tokens, i);
		check_redir_output(data, prompt, tokens, i);
		i++;
	}
	valid_pair_operands(data, prompt);
	logic_trans_args_cmd(data, tokens);
	return (0);
}

void	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	int	i;

	i = 0;
	allocate_tokens(data, prompt, input);
	get_tokens(data, prompt->tokens, input);
	while (i < prompt->n_tokens)
	{
		if (prompt->tokens[i].value)
			printf("Before Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
	valid_tokens(data, prompt, prompt->tokens);
	printf("------------------------------------------------\n");
	i = 0;
	while (i < prompt->n_tokens)
	{
		if (prompt->tokens[i].value)
			printf("After Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
}
