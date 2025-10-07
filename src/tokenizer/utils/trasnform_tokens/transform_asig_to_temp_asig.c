/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_asig_to_temp_asig.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 20:39:26 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 18:16:52 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/**
 * Este bloque de funciones se encarga de transformar asignaciones locales
 * en asignaciones temporales (`TEMP_ASIGNATION`) cuando el contexto indica
 * que no afectarán al entorno del proceso padre, sino que serán heredadas
 * únicamente por el proceso hijo que ejecuta el comando.
 *
 * En shells como Bash, una asignación como `VAR=1 ls` no persiste en el
 * entorno del shell padre, sino que se transmite temporalmente al hijo
 * que ejecuta `ls`. Este comportamiento se emula aquí.
 *
 * Función: verify_till_valid_token
 * --------------------------------
 * Recorre los tokens hacia adelante desde el índice `i` para verificar si
 * existe un delimitador (como `&&`, `|`, etc.) antes de encontrar un comando
 * que no sea `export`. Si se encuentra un delimitador, se considera que la
 * asignación está en un entorno válido. Si se encuentra un comando distinto
 * de `export`, se considera que la asignación será temporal.
 *
 * Función: check_if_temp_asig
 * ---------------------------
 * Evalúa si una asignación (`token`) debe ser considerada temporal.
 * Lo hace analizando el token siguiente:
 * - Si es un `WORD` y `verify_till_valid_token` confirma que no haydelimitador,
 *   se considera temporal.
 * - Si es un `COMMAND`, también se considera temporal.
 * - Si es otra `ASIGNATION` pero no está en un entorno válido, también lo es.
 * - Si es un `BUILT_IN` distinto de `export`, también se trata como temporal.
 *
 * Función: transform_asig_to_temp_asig
 * ------------------------------------
 * Recorre todos los tokens del prompt.Si encuentra una asignación(`ASIGNATION`)
 * que cumple las condiciones de `check_if_temp_asig`, la transforma en
 * `TEMP_ASIGNATION`. Esto permite que el executor sepa que dicha variable
 * debe eliminarse después de ejecutar el nodo correspondiente.
*/

static int	verify_till_valid_token(t_prompt *prompt, t_token *tokens, int i)
{
	while (i < prompt->n_tokens && tokens[i].type)
	{
		if (is_delimiter_type(tokens[i].type))
			return (SUCCESS);
		if (tokens[i].type == WORD)
			return (FALSE);
		i++;
	}
	return (SUCCESS);
}

static int	check_if_temp_asig(t_prompt *prompt, t_token *tokens, t_token *token)
{
	if (token->id < prompt->n_tokens)
	{
		if ((tokens[token->id + 1].type == WORD
			&& verify_till_valid_token(prompt, tokens, token->id))
		|| tokens[token->id + 1].type == COMMAND
		|| (is_asignation_type(tokens[token->id + 1].type)
			&& !verify_till_valid_token(prompt, tokens, token->id))
		|| tokens[token->id + 1].type == BUILT_IN
		|| tokens[token->id + 1].type == REDIR_OUTPUT)
		return (SUCCESS);
	}
	return (FALSE);
}

void	transform_asig_to_temp_asig(t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == ASIGNATION)
		{
			if (check_if_temp_asig(prompt, tokens, &tokens[i]))
				tokens[i].type = TEMP_ASIGNATION;
		}
		i++;
	}
}
