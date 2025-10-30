/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_syntaxis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:39:55 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 01:05:25 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta si un comando o built-in aparece justo después de un paréntesis
	cerrado. Si es así, lanza un error de sintaxis porque la estructura es
	inválida. Evita construcciones como ") ls", que no son válidas en el shell.
	Devuelve SUCCESS si no hay conflicto de sintaxis.
*/

int	check_cmd_syntax(t_shell *data, t_prompt *prompt, t_token *token, int i)
{
	(void)prompt;
	if (token[i].type == COMMAND || token[i].type == BUILT_IN)
	{
		if (i >= 1 && token[i - 1].type && token[i - 1].type == PAREN_CLOSE)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, token[i].value);
			return (SYNTAX_ERROR);
		}
	}
	return (SUCCESS);
}
