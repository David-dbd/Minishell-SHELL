/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:29:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 17:49:42 by migarrid         ###   ########.fr       */
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
	t_node	*central;

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
		central = create_true_node(data, COMMAND);
		if (!central)
			return (NULL);
		central->redir = get_redirs(data, tokens, i, TRUE);
		central->background = get_background(tokens, n_tokens, i);
		return (central);
	}
	return (NULL);
}

int	get_information(t_shell *data, t_token *tokens, int *i, t_node *central)
{
	int		start;

	start = *i;
	central->assig_tmp = get_temp_asignations(data, tokens, *i);
	if (data->error_state == TRUE)
		return (FAILURE);
	central->redir = get_redirs(data, tokens, i, COMMAND);
	if (data->error_state == TRUE || check_signal_node_heredoc(central))
		return (FAILURE);
	central->args = get_args_for_binary(data, tokens, i);
	if (data->error_state == TRUE)
		return (FAILURE);
	central->arg_types = get_arg_types(data, central, start, *i);
	if (data->error_state == TRUE)
		return (FAILURE);
	central->background = get_background(tokens, data->prompt.n_tokens, i);
	return (SUCCESS);
}

/*
	La razón por que el wildcard se pasa como comando es por el caso edge en el
	que *$pdw && ls se ejecute. Esto no se expandira sino hasta la ejecución,
	de modo que quedara como WILDCARD - NO_SPACE - EXPANSION....
	como bien se sabe, no existe un nodo wildcard y una ejecución en
	especifico en un AST y ademas bash no lo trata así de modo que simplemente
	se crea el nodo y luego pasa por execute_cmd, el cual expande el wildcard
	y luego intenta ejecutarlo, cosa que dara error y que esta bien. De otro
	modo el programa no haria absolutamnete nada (ast, ejecución, errores. etc).

	Cabe resaltar que el if (is_asignation_type(tokens[*i].type)) es para asig
	locales (si no mal recuerdo). Así, tenemos nodos para TEMP_ASIG Y LOCAL_ASIG

	En definitiva, parse_cmd no solo es para commands, sino para wildcards,
	asigs locales, redirs y cmd. Es una función versatil y muiltiuso.
*/

t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;

	central = NULL;
	central = special_cases(data, tokens, i, n_tokens);
	if (central)
		return (central);
	if (*i < n_tokens && tokens[*i].type
		&& (is_cmd_builtin_type(tokens[*i].type)
			|| is_real_assignation_type(tokens[*i].type)
			|| is_redir_type(tokens[*i].type) || tokens[*i].type == WILDCARD))
	{
		index_redir_input(tokens[*i].type, i, n_tokens);
		//expand_alias(data, tokens, *i); //verificar si el cmd es un alias o no, ver si se puede colocar en transform tokens
		central = create_node(data, &tokens[*i], tokens[*i].type);
		if (!central)
			return (NULL);
		if (is_asignation_type(tokens[*i].type))
			return (safe_index_plus(i, data->prompt.n_tokens), central);
		get_information(data, tokens, i, central);
		if (data->error_state == TRUE || check_signal_node_heredoc(central))
			return (clean_node(&central), NULL);
		return (central);
	}
	return (central);
}
