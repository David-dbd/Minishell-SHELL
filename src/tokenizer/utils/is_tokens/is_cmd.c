/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 21:35:11 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:15:46 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

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

static int	is_built_in(t_prompt *prompt, t_token *token, char *str)
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
		if (ft_strcmp(built_in[i], str) == 0)
		{
			token->type = BUILT_IN;
			prompt->n_cmds++;
			return (YES);
		}
		i++;
	}
	return (NO);
}

void	is_cmd(t_shell *data, t_prompt *prompt, t_token *token, char *str)
{
	char	**path_arr;
	char	*path_slash;
	char	*path;
	int		validate;

	path_slash = NULL;
	path = getenv("PATH");
	if (is_built_in(prompt, token, str) == YES)
		return ;
	path_arr = ft_split(path, ':');
	if (!path_arr)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	validate = find_bin(path_arr, path_slash, path, str);
	if (validate == SUCCESS)
	{
		token->type = COMMAND;
		prompt->n_cmds++;
		return ;
	}
	else if (validate == ERROR)
	{
		set_free(path_arr, &path_slash, &path);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
	return ;
}
