/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/22 03:08:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta redirecciones de salida '>', '>>' o entrada '<'.
	Añade el token correspondiente y ajusta el índice si es doble.
*/

void	is_redir_append(t_shell *data, t_token *tokens, const char *str, int *i)
{
	char	*append;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '>' && str[*i + 2] == '>')
	{
		append = ft_substr(str, *i, 3);
		if (!append)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, append, REDIR_APPEND);
		(*i) += 3;
	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		append = ft_strdup(">>");
		if (!append)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, append, REDIR_APPEND);
		(*i) += 2;
	}
}

void	is_redir_output(t_shell *data, t_token *tokens, const char *str, int *i)
{
	char	*output;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '>' && str[*i + 2] != '>')
	{
		output = ft_substr(str, *i, 2);
		if (!output)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, output, REDIR_OUTPUT);
		(*i) += 2;
	}
	else if (str[*i] == '>' && str[*i + 1] != '>')
	{
		output = ft_strdup(">");
		if (!output)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, output, REDIR_OUTPUT);
		(*i)++;
	}
}

void	is_redir_input(t_shell *data, t_token *tokens, const char *str, int *i)
{
	char	*input;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '<' && str[*i + 2] != '<')
	{
		input = ft_substr(str, *i, 2);
		if (!input)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, input, REDIR_INPUT);
		(*i) += 2;
	}
	else if (str[*i] == '<' && str[*i + 1] != '<')
	{
		input = ft_strdup("<");
		if (!input)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, input, REDIR_INPUT);
		(*i)++;
	}
}

void	is_redir(t_shell *data, t_token *tokens, const char *str, int *i)
{
	is_redir_append(data, tokens, str, i);
	is_redir_output(data, tokens, str, i);
	is_redir_input(data, tokens, str, i);
}
