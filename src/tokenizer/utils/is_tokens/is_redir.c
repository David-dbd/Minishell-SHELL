/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:16:43 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta redirecciones de salida '>', '>>' o entrada '<'.
	Añade el token correspondiente y ajusta el índice si es doble.
*/

void	is_redir(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str + *i, ">>", 2))
	{
		add_token(tokens, ">>", REDIR_APPEND);
		(*i)++;
		(*i)++;
	}
	else if (str[*i] == '>')
	{
		add_token(tokens, ">", REDIR_OUTPUT);
		(*i)++;
	}
	else if (str[*i] == '<')
	{
		add_token(tokens, "<", REDIR_INPUT);
		(*i)++;
	}
}
