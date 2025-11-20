/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_tilde_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:16:04 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 23:55:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta si la clave representa una variable especial del shell.
	Si es así, la expande usando su valor correspondiente:

	~ -> HOME
	~+ -> PWD

	El flujo es:

	Se intenta hallar el HOME en el env. Si no se encuntra entonces se lanza
	error. Si se encuentra, entonces se verifica su entorno. Si lo que tiene
	al lado no es un cmd o built_in entonces falla.
	Luego se pasa a copy_value para su expansión. si por algun motivo el valor
	de HOME existe pero vacio, entonces simplemente se lanza error y luego false
	para que expnad_empty_str pueda expandir a vacio.

	En tilde_plus pasa casi lo mismo. Excepto que si no se halla en el env,
	se usa el getcwd. El mismo comportamiento de pwd
*/

static void	expand_tilde(t_shell *d, t_token *t, char *value_to_copy, char *key)
{
	if (t->value)
		free (t->value);
	t->value = ft_strdup(value_to_copy);
	if (!t->value)
	{
		free (key);
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	}
}

static int	is_just_tilde(t_shell *data, t_token *token, char **key_to_find)
{
	char	*value;

	free (*key_to_find);
	*key_to_find = ft_strdup("HOME");
	if (!*key_to_find)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	value = get_var_value(data->env.vars, *key_to_find);
	if (!value)
	{
		free (*key_to_find);
		*key_to_find = NULL;
		ft_printf_fd(STDERR, ERR_HOME_NOT_SET); //Im not doung the whole USERS/name: Is a directory cuz I dont have the function to retrieve the Home value
		return (ERROR);
	}
	expand_tilde(data, token, value, *key_to_find);
	return (TRUE);
}

static int	is_tilde_plus(t_shell *data, t_token *token, char **key_to_find)
{
	char	cwd[PATH_MAX];
	char	*value;

	free (*key_to_find);
	*key_to_find = ft_strdup("PWD");
	if (!*key_to_find)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	value = get_var_value(data->env.vars, *key_to_find);
	if (!value) //-Bash saca el PWD desde el env o lo saca con getcwd? el resultado varia porque se puede borrar en env
	{
		if (!getcwd(cwd, sizeof(cwd)))
		{
			free (*key_to_find);
			*key_to_find = NULL;
			return (perror("minishell: cd: "), ERROR); //codigo de error de getcwd?
		}
		expand_tilde(data, token, cwd, *key_to_find);
		return (TRUE);
	}
	expand_tilde(data, token, value, *key_to_find);
	return (TRUE);
}

static int	is_tilde_minus(t_shell *data, t_token *token, char **key_to_find)
{
	char	cwd[PATH_MAX];
	char	*value;

	free (*key_to_find);
	*key_to_find = ft_strdup("OLDPWD");
	if (!*key_to_find)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	value = get_var_value(data->env.vars, *key_to_find);
	if (!value) //-Bash saca el PWD desde el env o lo saca con getcwd? el resultado varia porque se puede borrar en env
	{
		free (*key_to_find);
		*key_to_find = NULL;
		ft_printf_fd(STDERR, ERR_OLDPWD_NOT_SET); //Im not doung the whole USERS/name: Is a directory cuz I dont have the function to retrieve the Home value
		return (ERROR);
	}
	expand_tilde(data, token, value, *key_to_find);
	return (TRUE);
}

int	is_it_tilde(t_shell *data, t_token *token, char **key_to_find)
{
	int	result;

	result = FALSE;
	if (key_to_find[0][0] == '~' && key_to_find[0][1] == '\0')
	{
		result = is_just_tilde(data, token, key_to_find);
	}
	else if (key_to_find[0][0] == '~' && key_to_find[0][1] == '+'
		&& key_to_find[0][2] == '\0') //update the copy_key to take the tilde as well
	{
		result = is_tilde_plus(data, token, key_to_find);
	}
	else if (key_to_find[0][0] == '~' && key_to_find[0][1] == '-'
		&& key_to_find[0][2] == '\0')
	{
		result = is_tilde_minus(data, token, key_to_find);
	}
	return (result);
}
