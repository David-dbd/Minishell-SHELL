/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:05 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 00:21:55 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Se compara con todos los cmd que haya en la linked list.
	Si se hace match con su alias entonces se remplaza el
	token->value por el cmd->value;

	Hay que recordar que aqui el token->value es el alias que el usuario
	introduce, el cual hay que encontrar en la lista. Cuando se encuentre,
	se verifica si es un built_in o si hay que dejarlo como cmd.
	El cmd->value es "cd", "echo", "unset", etc...
*/

static int	find_alias(t_shell *data, t_cmd *cmd, t_token *token)
{
	while (cmd)
	{
		if (cmd->alias)
		{
			if (ft_strcmp(token->value, cmd->alias) == 0) //se compara el alias que puso el usuario con los nodos
			{
				if (is_built_in(data, NULL, cmd->value)) //si es el alias de un built_in entonces se cambia su token a built_in
					token->type = BUILT_IN;
				if (token->value) //No hace falta esete if pero lo dejo por seguridad?
					free (token->value);
				token->value = ft_strdup(cmd->value);
				if (!token->value)
					return (ERROR);
				return (SUCCESS);
			}
		}
		cmd = cmd->next;
	}
	return (FAILURE);
}

/*
	Si el token es un comando entonces se envia a find_alias para verificar si
	es un alias seteado anteriormente.
	EDITAR PARA QUE ACEPTE TANTO COMMANDS COMO WORDS
*/

void	expand_alias(t_shell *data, t_token *tokens, int i)
{
	int	result;

	if (tokens[i].type == COMMAND)
	{
		result = find_alias(data, data->extras.cmd, &tokens[i]);
		if (result == ERROR)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
}
