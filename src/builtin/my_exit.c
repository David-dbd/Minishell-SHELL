/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:55:15 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/08 16:55:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Cuenta los argumentos del "exit".
*/

static int	counter_args(char **args)
{
	int	n_args;

	n_args = 0;
	if (!args)
		return (0);
	while (args[n_args])
		n_args++;
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

static int	check_exit(t_shell *data, char **args)
{
	long	num;
	int		status;
	int		n_args;

	n_args = counter_args(args);
	if (n_args > 1)
		return (ERROR);
	if (n_args == 1)
	{
		if (is_numeric(args[0]))
		{
			if (!ft_atol_range(args[0]))
				return (ft_printf_fd(STDERR, ERR_LIMIT_LONG, args[0]), EXIT_USE);
			num = ft_atol(args[0]);
			status = ((num % 256) + 256) % 256;
			return (status);
		}
		exit_error(data, ERR_EXIT_NUMERIC, EXIT_USE, args[0]);
	}
	return (0);
}

/*
	Ejecuta el built-in "exit" con el status validado.
*/

void	my_exit(t_shell *data, char **args)
{
	int	status;

	status = check_exit(data, args);
	if (status == ERROR)
	{
		data->exit_code = 2;
		ft_printf_fd(STDERR, ERR_EXIT_TOO_MANY);
		return ;
	}
	exit_succes(data, NULL, status);
}
