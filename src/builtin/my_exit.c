/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:55:15 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/17 17:50:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Cuenta los argumentos tipo WORD que siguen a "exit".
*/

static int	counter_args(t_token *token, int n_tokens)
{
	int	i;
	int	n_args;

	i = 1;
	n_args = 0;
	while (token->id + i < n_tokens
		&& token[+i].type
		&& (!is_delimiter_type((token[+i].type))
			&& !is_redir_type(token[+i].type)))
	{
		if (token[+i].type == WORD)
			n_args++;
		i++;
	}
	return (n_args);
}

/*
	Devuelve SUCCESS si la cadena es numérica (+/- opcional),
	FAILURE en caso contrario.
*/

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (FAILURE);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (FAILURE);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

/*
	Valida los argumentos de "exit":
	- Más de 1 → error "too many arguments".
	- 1 argumento válido → devuelve valor dividido por el modulo de 256.
	- 1 argumento no numérico → error.
	- Sin argumentos → devuelve 0.
*/

static int	check_exit(t_shell *data, t_token *token, int n_tokens)
{
	long	num;
	int		status;
	int		n_args;
	int		i;

	i = 1;
	n_args = counter_args(token, n_tokens);
	if (n_args > 1)
		exit_error(data, ERR_EXIT_TOO_MANY, EXIT_USE);
	if (n_args == 1)
	{
		if (is_numeric(token[+i].value))
		{
			num = ft_atol(token[+i].value);
			status = num % 256;
			return (status);
		}
		exit_error(data, ERR_EXIT_NUMERIC, EXIT_USE, token[+1].value);
	}
	return (0);
}

/*
	Ejecuta el built-in "exit" con el status validado.
*/

void	my_exit(t_shell *data, t_prompt *prompt, t_token *token)
{
	int	status;

	status = check_exit(data, token, prompt->n_tokens);
	exit_succes(data, NULL, status);
}
