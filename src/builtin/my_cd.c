/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 17:12:22 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/03 21:50:34 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_args(t_token *tokens, int i, int n_tokens)
{
	int	count;

	count = 0;
	while (i < n_tokens && tokens[i].type == WORD)
	{
		i++;
		count++;
	}
	return (count);
}

static void	update_pwd(t_shell *data, char *cwd, char *key_to_find)
{
	t_var	*var;
	int		flag_found;

	flag_found = FALSE;
	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key_to_find) == 0)
		{
			flag_found = TRUE;
			if (var->value)
				free (var->value);
			var->value = ft_strdup(cwd);
			if (!var->value)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			break ;
		}
		var = var->next;
	}
	if (!flag_found)
		add_var(data, key_to_find, cwd, ENV);
}

//EN OBRAS...no esta listo
int	my_cd(t_shell *data, t_token *tokens, t_token *token)
{
	char	new_cwd[PATH_MAX];
	char	cwd[PATH_MAX];
	int		i;
	int		args_found;
	struct	stat info;

	i = token->id;
	args_found = FALSE;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_printf_fd(STDOUT, "un error por aquiii");
		return (EXIT_USE);
	}
	if (tokens[i].type == BUILT_IN)
		i++;
	args_found = count_args(tokens, i, data->prompt.n_tokens);
	if (!args_found)
	{
		data->home = getenv("HOME");
		if (!data->home || chdir(data->home) == -1)
		{
			ft_printf_fd(STDOUT, ERR_CD_NO_HOME);
			return (EXIT_USE);
		}
		return (SUCCESS);
	}
	else if (args_found > 1)
	{
		ft_printf_fd(STDOUT, ERR_CD_TOO_MANY);
		return (EXIT_USE);
	}
	//if (tokens[i].value[0] == '-')
	///	if ()
	//}
	if (stat(tokens[i].value, &info) == ERROR)
	{
		ft_printf_fd(STDOUT, ERR_FILE_NOT_FOUND);
		return (EXIT_USE);
	}
	if (!S_ISDIR(info.st_mode))
	{
		ft_printf_fd(STDOUT, ERR_NOT_DIR);
		return (ENOTDIR);
	}
	if (access(tokens[i].value, R_OK) == ERROR)
	{
		ft_printf_fd(STDOUT, ERR_PERM_DENIED);
		return (EACCES);
	}
	if (chdir(tokens[i].value) == -1)
	{
		perror("minishell: chdir: ");
		return (EXIT_USE);
	}
	if (getcwd(new_cwd, sizeof(new_cwd)))
	{
		update_pwd(data, new_cwd, "PWD");
		update_pwd(data, cwd, "OLDPWD");
	}
	return (SUCCESS);
	//falla con /MINISHELL
	//cd ../../../
	//cd 42 desde el home
	//falla en escenarios random. Tal vez sea como lo escribi
	//falla desde el home casi siempre
	//averiguar si puedo sacar el home de envp
}
