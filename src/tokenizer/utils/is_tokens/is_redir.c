/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:41 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta redirección de append '>>'.
	- Soporta descriptor antes (ej. '2>>').
	- Crea token REDIR_APPEND y avanza el índice.
*/

void	is_redir_append(t_shell *data, t_prompt *promp, const char *str, int *i)
{
	char	*append;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '>' && str[*i + 2] == '>')
	{
		append = ft_substr(str, *i, 3);
		if (!append)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, append, REDIR_APPEND);
		(*i) += 3;
	}
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		append = ft_strdup(">>");
		if (!append)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, append, REDIR_APPEND);
		(*i) += 2;
	}
}

/*
	Detecta redirección de salida '>'.
	- Soporta descriptor antes (ej. '2>').
	- Crea token REDIR_OUTPUT y avanza el índice.
*/

void	is_redir_output(t_shell *data, t_prompt *promp, const char *str, int *i)
{
	char	*output;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '>' && str[*i + 2] != '>')
	{
		output = ft_substr(str, *i, 2);
		if (!output)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, output, REDIR_OUTPUT);
		(*i) += 2;
	}
	else if (str[*i] == '>' && str[*i + 1] != '>')
	{
		output = ft_strdup(">");
		if (!output)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, output, REDIR_OUTPUT);
		(*i)++;
	}
}

/*
	Detecta redirección noclobber '>|'.
	- Soporta descriptor antes (ej. '2>|').
	- Crea token REDIR_OUTPUT y avanza el índice.
*/

void	is_redir_noclobber(t_shell *d, t_prompt *promp, const char *str, int *i)
{
	char	*no_clobber;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '>' && str[*i + 2] == '|')
	{
		no_clobber = ft_substr(str, *i, 3);
		if (!no_clobber)
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		add_token(d, promp, no_clobber, REDIR_OUTPUT);
		(*i) += 3;
	}
	else if (str[*i] == '>' && str[*i + 1] == '|')
	{
		no_clobber = ft_substr(str, *i, 2);
		if (!no_clobber)
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		add_token(d, promp, no_clobber, REDIR_OUTPUT);
		(*i) += 2;
	}
}

/*
	Detecta redirección de entrada '<'.
	- Soporta descriptor antes (ej. '2<').
	- Crea token REDIR_INPUT y avanza el índice.
*/

void	is_redir_input(t_shell *data, t_prompt *promp, const char *str, int *i)
{
	char	*input;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '<' && str[*i + 2] != '<')
	{
		input = ft_substr(str, *i, 2);
		if (!input)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, input, REDIR_INPUT);
		(*i) += 2;
	}
	else if (str[*i] == '<' && str[*i + 1] != '<')
	{
		input = ft_strdup("<");
		if (!input)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, promp, input, REDIR_INPUT);
		(*i)++;
	}
}

/*
	Llama a todas las funciones de redirección para procesar
	cualquier tipo de operador '<', '>', '>>' o '>|'.
*/

void	is_redir(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	is_redir_noclobber(data, prompt, str, i);
	is_redir_append(data, prompt, str, i);
	is_redir_output(data, prompt, str, i);
	is_redir_input(data, prompt, str, i);
}
