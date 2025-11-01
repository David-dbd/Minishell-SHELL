/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_it_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 12:42:41 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 12:42:59 by davdiaz-         ###   ########.fr       */
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
	if (value_of_symbol_expansion)
		free (value_of_symbol_expansion);
}

/*
	Detecta si la clave representa una variable especial del shell.
	Si es así, la expande usando su valor correspondiente:

	~ -> HOME
	~+ -> PWD
*/

static int is_it_tilde(t_shell *data, t_token *token, char **key_to_find)
{
	if (key_to_find[0][0] == '~' && key_to_find[0][1] == '\0')
	{
		free (*key_to_find);
		*key_to_find = ft_strdup("HOME");
		if (!*key_to_find)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		if (ft_strlen(token->value) > 1)
		{
			expand_empty_str(data, token, key_to_find);
			return (TRUE);
		}
		if (find_key_in_lst(data, token, key_to_find))
			return (TRUE);
		ft_printf_fd(STDERR, ERR_HOME_NOT_SET);
	}
	else if (key_to_find[0][0] == '+' && key_to_find[0][1] == '\0')
	{
		free (*key_to_find);
		*key_to_find = ft_strdup("PWD");
		if (!*key_to_find)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		if (ft_strlen(token->value) > 1)
		{
			expand_empty_str(data, token, key_to_find);
			return (TRUE);
		}
		if (find_key_in_lst(data, token, key_to_find))
			return (TRUE);
		ft_printf_fd(STDERR, "PWD NOT SET");
	}
	return (FALSE);
}

/*
	Detecta si la clave representa una variable especial del shell.
	Si es así, la expande usando su valor correspondiente:

	$$ → PID del shell actual (shell padre)
	$! → PID del último proceso en segundo plano (hijo)
	$? → Código de salida del último proceso ejecutado
*/

int	is_it_symbol(t_shell *data, t_token *token, char **key_to_find)
{
	if (key_to_find[0][0] == '!' && key_to_find[0][1] == '\0')
	{
		if (data->ast_root->pid)
		{
			sym_expa(data, token, key_to_find, data->ast_root->pid);
			return (TRUE);
		}
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
	else if (is_it_tilde(data, token, key_to_find))
	{
		free (*key_to_find);
		return (TRUE);
	}
	return (FALSE);
}
