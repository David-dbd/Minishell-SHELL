/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:29:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/03 01:43:41 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Este bloque de funciones forma parte del parser encargado de
	construir nodos CMD a partir de los tokens del prompt. Su objetivo
	es interpretar correctamente comandos, asignaciones y redirecciones.

	La función `parse_cmd` es el punto de entrada principal. Antes de
	procesar el token actual, llama a `special_cases` para detectar
	patrones especiales como redirecciones sin comando o asignaciones
	temporales.

	Si el token es una redirección, se crea un nodo `true` como
	placeholder. Esto permite que redirecciones como `> archivo.txt`
	funcionen incluso sin comando explícito.

	Si no hay casos especiales, `parse_cmd` verifica si el token actual
	representa un comando válido o una asignación real. Si es así, crea
	el nodo y llama a `get_information`.

	La función `get_information` completa el nodo con redirecciones,
	argumentos, tipos de argumentos y si el comando va en segundo plano.

	Este diseño modular permite interpretar correctamente líneas como:
	`export VAR=1 > archivo.txt &` o `> archivo.txt`, manteniendo la
	semántica y estructura de cada componente.
*/

void	index_redir_input(int type, int *i, int n_tokens)
{
	if (is_redir_type(type))
	{
		safe_index_plus(i, n_tokens);
		safe_index_plus(i, n_tokens);
	}
}

t_node	*special_cases(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*left;

	if (*i == n_tokens)
		return (NULL);
	while ((*i) < n_tokens && (tokens[*i].type == TEMP_ASIGNATION
			|| tokens[*i].type == TEMP_PLUS_ASIGNATION))
	{
		(*i)++;
	}
	if ((*i + 1 == n_tokens && is_redir_type(tokens[*i].type))
		|| (*i + 2 == n_tokens && is_redir_type(tokens[*i].type))
		|| (*i + 2 < n_tokens && is_redir_type(tokens[*i].type)
			&& !is_cmd_builtin_type(tokens[*i + 2].type)))
	{
		left = create_true_node(data, COMMAND);
		left->redir = get_redirs(data, tokens, i, TRUE);
		left->background = get_background(tokens, n_tokens, i);
		return (left);
	}
	return (NULL);
}

int	get_information(t_shell *data, t_token *tokens, int *i, t_node *left)
{
	int		start;

	start = *i;
	// expansion(data, tokens, FINAL_PHASE);
	left->assig_tmp = get_temp_asignations(data, tokens, *i);
	left->redir = get_redirs(data, tokens, i, COMMAND);
	left->args = get_args_for_binary(data, tokens, i);
	left->arg_types = get_arg_types(data, left, start, *i);
	left->background = get_background(tokens, data->prompt.n_tokens, i);
	return (SUCCESS);
}

t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*left;

	left = NULL;
	left = special_cases(data, tokens, i, n_tokens);
	if (left)
		return (left);
	if (*i < n_tokens && tokens[*i].type
		&& (is_cmd_builtin_type(tokens[*i].type)
			|| is_real_assignation_type(tokens[*i].type)
			|| is_redir_type(tokens[*i].type)))
	{
		index_redir_input(tokens[*i].type, i, n_tokens);
		//expand_alias(data, tokens, *i); //verificar si el cmd es un alias o no, ver si se puede colocar en transform tokens
		left = create_node(data, &tokens[*i], tokens[*i].type);
		if (is_asignation_type(tokens[*i].type))
		{
			safe_index_plus(i, data->prompt.n_tokens);
			return (left);
		}
		get_information(data, tokens, i, left);
		if (left->redir && check_signal_node_heredoc(left, left->redir))
			return (NULL);
		return (left);
	}
	return (left);
}
