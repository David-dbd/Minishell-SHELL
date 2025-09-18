/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_key_in_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:17:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 02:38:30 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

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
}

/*
	Detecta si la clave representa una variable especial del shell.
	Si es así, la expande usando su valor correspondiente:

	$$ → PID del shell actual (shell padre)
	$! → PID del último proceso en segundo plano (hijo)
	$? → Código de salida del último proceso ejecutado
*/

static int	is_it_symbol(t_shell *data, t_token *token, char **key_to_find)
{
	if (*key_to_find[0] == '!' && *key_to_find[1] == '\0')
	{
		if (data->ast_root->pid)
		{
			sym_expa(data, token, key_to_find, data->ast_root->pid);
			return (TRUE);
		}
		else
			return (FALSE);
	}
	if (*key_to_find[0] == '?' && *key_to_find[1] == '\0')
	{
		if (data->exit_code)
		{
			sym_expa(data, token, key_to_find, data->shell_pid);
			return (TRUE);
		}
	}
	else if (*key_to_find[0] == '$' && *key_to_find[1] == '\0')
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
*/

int	find_key_in_lst(t_shell *data, t_token *token, char **key_to_find)
{
	t_var	*var;
	int		match_for_symbol;

	var = data->env.vars;
	while (var != NULL)
	{
		if (ft_strcmp(var->key, *key_to_find) == 0)
		{
			token->type = WORD;
			copy_value(data, &token->value, var->value, *key_to_find);
			return (TRUE);
		}
		var = var->next;
	}
	match_for_symbol = is_it_symbol(data, token, key_to_find);
	if (match_for_symbol)
		return (TRUE);
	return (FALSE);
}
