/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_alias_part_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:42 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 00:25:14 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Si el alias que intenta asignarse ya existe en otro cmd entonces
	se libera el alias y se asigna al que se esta intentando.

	Eg: alias hola cd -> ya existe hola para echo.
	Asi que se libera y se asigna a cd.
*/

static void	check_if_already_assing(t_cmd *cmd, char *alias)
{
	while (cmd)
	{
		if (cmd->alias)
		{
			if (ft_strcmp(cmd->alias, alias) == 0)
			{
				free (cmd->alias);
				cmd->alias = NULL;
			}
		}
		cmd = cmd->next;
	}
}

/*
	Revisa que el alias no sea un comando o built_in existente.
*/

static int	check_alias_conflicts(t_shell *data, char *alias)
{
	if (is_built_in(data, NULL, alias)
		|| get_path(data, alias, data->env.envp)) //leak porque no se libera alias y cmd_to_find. Quitar el return exit de alli o duplicar la funcion pero sin el return.
	{
		ft_printf_fd(STDERR, ERR_BUILTIN_ALIAS, alias);
		return (TRUE);
	}
	return (FALSE);
}

int	find_cmd(t_shell *data, t_cmd *cmd, char *to_find, char *alias)
{
	while (cmd)
	{
		if (ft_strcmp(cmd->value, to_find) == 0)
		{
			check_if_already_assing(cmd, alias); //si ya esta asignado a otro cmd entonces se libera para ese cmd. Preguntar que se prefiere. Personalmente me da igual.
			if (check_alias_conflicts(data, alias) == TRUE) //Si el alias que intenta asignarse es un cmd existente
			{
				free (alias);
				free (to_find);
				return (ERROR);
			}
			if (cmd->alias && ft_strcmp(cmd->alias, alias) != 0) //si el alias es diferente al que el usauio introduce
			{
				free (cmd->alias);
				cmd->alias = alias;
			}
			else
				free (alias);// si no son diferentes: hola - hola, se libera y no se asigna
			free (to_find);
			return (TRUE);
		}
		cmd = cmd->next;
	}
	return (FALSE);
}
