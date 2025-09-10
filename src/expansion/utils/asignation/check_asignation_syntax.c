/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_asignation_syntax.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:34:34 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/10 17:38:12 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*

	Asignaciones solo pueden tener && || | asignaciones detras y delante para funcionar. Nada de BUILT-INS o CMD o WORDS
	Si tiene asignaciones - cmd ignora la asignacion.
	si tiene cmd asignacion - es parte argumento
	Export funciona ignorarndo todo lo que no sea asignacion.
	Si export tiene detras un cmd, es WORD
	Si export tiene detras un word o cmd falla
	Propósito:
	*Verifica si la sintaxis interna del token es válida para una asignación.

	Lógica:
	*El nombre de la variable debe comenzar con una letra o guion bajo.
	*No debe contener espacios, tabulaciones ni caracteres de control.expo
	export && hola -> bash: hola: command not found
	export=jaja=jiji -> hace export de todo el str:
	bash-3.2$ echo $export=jaja=jiji
	bash-3.2$ jaja=jiji=jaja=jiji
*/

static int count_syntax(t_token *token)
{
	int i;
	int	check_for_equal;
	int	before_text;
	int after_text;

	check_for_equal = 0;
	before_text = 0;
	after_text = 0;
	while (token->value[i] != '\0')
	{
		if (check_for_equal == 0)
			before_text++;
		else if (check_for_equal > 0)
			after_text++;
		if (ft_isspace(token->value[i]))
			return (FALSE);
		if (token->value[i] == '=')
			check_for_equal++;
		i++;
	}
	if (!check_for_equal || !before_text || !after_text)
		return (FALSE);
	return (TRUE);
}

int check_asignation_syntax(t_token *token)
{
	int	i;
	int	len;
	int	result;

	i = 0;
	len = ft_strlen(token->value);
	if (len < 1)
		return (FALSE);
	if (!ft_isalpha(token->value[i]) && token->value[i] != '_')
		return (FALSE);
	result = count_syntax(token);
	return (result);
}

