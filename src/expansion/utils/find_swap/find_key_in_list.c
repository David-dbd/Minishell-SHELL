/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_key_in_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/29 02:31:09 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

char *find_value_key(t_shell *data, t_var *vars, char *key_to_find)
{
	char	*name;
	t_var 	*var;

	var = vars;
	name = NULL;
	while(var)
	{
		if (ft_strcmp(var->key, key_to_find) == 0)
		{
			name = var->key;
			return(name);
		}
		var = var->next;
	}
	return (name);
}

static void	sym_expa(t_shell *d, t_token *token, char **key_to_f, int sym_value)
{
	char	*value_of_symbol_expansion;

	value_of_symbol_expansion = ft_itoa(sym_value);
	if (!value_of_symbol_expansion)
	{
		free (*key_to_f);
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	}
	copy_value(d, &token->value, value_of_symbol_expansion, *key_to_f);
	if (value_of_symbol_expansion)
		free (value_of_symbol_expansion);
}

/*
	Detecta si la clave representa una variable especial del shell.
	Si es así, la expande usando su valor correspondiente:

	$$ → PID del shell actual (shell padre)
	$? → Código de salida del último proceso ejecutado
*/

static int	is_it_symbol(t_shell *data, t_token *token, char **key_to_find)
{
	if (key_to_find[0][0] == '!' && key_to_find[0][1] == '\0')
	{
		if (data->last_background_pid)
		{
			sym_expa(data, token, key_to_find, data->last_background_pid);
			return (TRUE);
		}
		else
			return (FALSE);
	}
	else if (key_to_find[0][0] == '?' && key_to_find[0][1] == '\0')
	{
		sym_expa(data, token, key_to_find, data->exit_code);
		return (TRUE);
	}
	else if (key_to_find[0][0] == '$' && key_to_find[0][1] == '\0')
	{
		sym_expa(data, token, key_to_find, data->shell_pid);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Intenta hayar la clave de la expansión en el env. Si la encuentra, llama a
	"copy_value" para reemplazar el $USER por su valor.
	Si no encuentra ninguna coincidencia, intenta hacer un match con algún
	simbolo: $! - $$ - $?.

	Debe de ser != EXP ya que cuando la variable es de ese tipo no tiene value,
	de modo que si se intentase expander su valor, daria como resultado
	expander datos basura

	No queremos expandir casos como var->type EXP porque no tiene ningun valor
	a expandir, por lo que hay que procesarlo de otra manera en la phase 2.
	Tampoco queremos expandir casos como TEMP_ASINATION ya que el padre no
	puede tener acceso a esos tokens, ya que son unicamente para los hijos.
	Es por esto, que aunque nuestra lista enlazada contiene todos los tipos
	de variables, el padre solo es capaz de acceder a LOCAL y ENV, mientras que
	puede expandir las mismas además de la ya descritas temp y exp (vacias)
*/

int	find_key_in_lst(t_shell *data, t_token *token, char **key_to_find)
{
	t_var	*var;
	int		match_for_symbol;

	var = data->env.vars;
	match_for_symbol = is_it_symbol(data, token, key_to_find);
	if (match_for_symbol)
		return (TRUE);
	while (var != NULL)
	{
		// printf("key_to_find: %s\n\n", *key_to_find);
		// printf("var->key: %s\n\n", var->key);
		// printf("var->value: %s\n\n", var->value);
		if (ft_strcmp(var->key, *key_to_find) == 0 && var->type != EXP
			&& var->type != TEMP_ASIGNATION)
		{
			// printf("FOUND key to expand");
			token->type = WORD;
			copy_value(data, &token->value, var->value, *key_to_find);
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
}
