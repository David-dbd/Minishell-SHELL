/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_tokens_logic.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 21:33:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/27 13:27:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Proceso de validación y transformación de tokens relacionados
 * con asignaciones.
 *
 * La primera fase consiste en recorrer todos los tokens de tipo
 * WORD y transformarlos en ASIGNATION siempre que su sintaxis
 * interna sea válida como asignación.
 *
 * Luego, se evalúan todos los tokens ASIGNATION para verificar
 * si, además de tener una sintaxis válida, su contexto también
 * lo permite. Al igual que "ls" puede ser un comando o un
 * argumento dependiendo de su posición, las asignaciones también
 * deben ser analizadas en función de su entorno. Si no cumplen
 * las condiciones contextuales, se transforman nuevamente en WORD.
 *
 * En la segunda fase, se revisan únicamente las asignaciones que
 * han sobrevivido a la primera pasada, pero de forma más
 * exhaustiva. Esto permite validar casos más complejos y detectar
 * patrones adicionales relacionados con asignaciones.
 *
 * Luego verifica tokens que esten desactualizados en FINAL_PAHSE de
 * transform_cmd_to_word
 *
 *
 * Pasando a asig_plus, se analizan las asignaciones restantes para
 * identificar posibles concatenaciones como "var+=hola". Si se
 * detecta este patrón, el tipo del token se transforma de
 * ASIGNATION a PLUS_ASIGNATION, lo que permite trabajar con
 * información más precisa, limpia y confiable.
 *
 * Para finalizar, se verifica cada ASIGNATION para ver si es temporal (TEMP)
 * (para un child process) o no, manteniendo así su tipo ASIGNATION.
 *
*/

void	transform_tokens_logic(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	transform_cmd_to_built_in(data, prompt, tokens);
	transform_cmd_to_word(data, tokens, INITIAL_PHASE);
	transform_word_to_asignation(data, tokens, INITIAL_PHASE);
	transform_word_to_asignation(data, tokens, FINAL_PHASE);
	transform_cmd_to_word(data, tokens, FINAL_PHASE);
	transform_invalid_asig_to_word(prompt, tokens);
	transform_asig_to_asig_plus(prompt, tokens);
	transform_asig_to_temp(data, prompt, tokens);
	transform_word_to_file(prompt, tokens);
	transform_command_built_lowercase(prompt, tokens);
	transform_cmd_to_built_in(data, prompt, tokens); //expor't -> export:cmd
}
