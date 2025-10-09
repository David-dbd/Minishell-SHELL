/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 21:35:11 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/08 15:49:41 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Liberar memoria de manera centralizada para path_arr, path_slash y path.
	- Evita fugas de memoria en find_bin.
*/

static void	set_free(char **path_arr, char **path_slash, char **path)
{
	ft_free_str_array(path_arr);
	if (*path_slash)
		free (*path_slash);
	*path_slash = NULL;
	if (*path)
		free (*path);
	*path = NULL;
}

/*
	Comprueba si el path existe en el sistema.
	- Si existe, libera memoria y devuelve TRUE.
	- Si no, devuelve FALSE para seguir buscando.
*/

static int	check_access(char **path_arr, char *path_slash, char *path)
{
	int	validate;

	validate = access(path, F_OK);
	if (validate == OK)
	{
		set_free(path_arr, &path_slash, &path);
		return (TRUE);
	}
	return (FALSE);
}

/*
	Busca un ejecutable en cada directorio del PATH.
	- path_arr: lista de directorios del PATH.
	- path_slash: directorio + "/" temporal.
	- path: directorio + "/" + palabra (posible comando).
	- word: nombre del comando a buscar.
	- Retorna SUCCESS si lo encuentra, NOT_FOUND si no, ERROR si falla malloc.
	- Usa check_access para validar existencia y set_free para limpiar.
*/

static int	find_bin(char **path_arr, char *path_slash, char *path, char *word)
{
	int	i;

	i = 0;
	while (path_arr[i] != NULL)
	{
		path_slash = ft_strjoin(path_arr[i], "/");
		if (!path_slash && i == 0)
			return (ERROR);
		else if (!path_slash && i > 0)
			return (set_free(path_arr, &path_slash, &path), ERROR);
		path = ft_strjoin(path_slash, word);
		if (!path)
			return (set_free(path_arr, &path_slash, &path), ERROR);
		if (check_access(path_arr, path_slash, path) == TRUE)
			return (SUCCESS);
		free(path_slash);
		free(path);
		path_slash = NULL;
		path = NULL;
		i++;
	}
	set_free(path_arr, &path_slash, &path);
	return (NOT_FOUND);
}

/*
	Comprueba si la palabra es un built-in conocido.
	- built_in: lista de comandos internos.
	- Si coincide, marca token->type = BUILT_IN
	- Devuelve YES si es built-in, NO si no lo es.
	- Usa ft_strmatch_cmp: Tranforma s1 para que haga match con s2 por si el
	usuario hace: Ls - Echo - ECHO - LS - CD - cd - etc...
*/

static int	is_built_in(t_shell *d, t_prompt *prompt, t_token *token, char *str)
{
	char	*built_in[8];
	int		i;

	i = 0;
	built_in[0] = "cd";
	built_in[1] = "echo";
	built_in[2] = "export";
	built_in[3] = "unset";
	built_in[4] = "env";
	built_in[5] = "exit";
	built_in[6] = "pwd";
	built_in[7] = NULL;
	while (built_in[i])
	{
		if (ft_strmatch_cmp(built_in[i], str) == 0)
		{
			token->type = BUILT_IN;
			return (YES);
		}
		i++;
	}
	return (NO);
}

/*
	Determina si un token es un comando ejecutable o built-in.
	- Primero revisa built-ins con is_built_in.
	- Luego busca en PATH si no es built-in.
	- Si PATH no existe por no env, crea un PATH por defecto.
	- Actualiza token->type = COMMAND si lo encuentra.
	- Maneja errores de memoria con set_free y exit_error.
*/

void	is_cmd(t_shell *data, t_prompt *prompt, t_token *token, char *str)
{
	char	**path_arr;
	char	*path_slash;
	char	*path;
	int		validate;

	path_slash = NULL;
	path = getenv("PATH");
	if (is_built_in(data, prompt, token, str) == YES)
		return ;
	if (!path)
		path_null_no_env(data, &path);
	path_arr = ft_split(path, ':');
	if (!path_arr)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	validate = find_bin(path_arr, path_slash, path, str);
	if (validate == SUCCESS)
		return (token->type = COMMAND, (void)OK);
	else if (validate == ERROR)
	{
		set_free(path_arr, &path_slash, &path);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
	return ;
}
