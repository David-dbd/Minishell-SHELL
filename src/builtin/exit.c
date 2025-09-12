/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:55:15 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/11 22:30:30 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	counter_args(t_token *tokens)
{
	int i;
	int n_args;

	i = 1;
	n_args = 0;
	while(tokens[i].type && !is_delimiter_type((tokens[i].type)))
	{
		if (tokens[i].type == WORD)
			n_args++;
		i++;
	}
	return (n_args);
}

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
	while(str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static int check_exit(t_shell *data, t_token *tokens)
{
	long	num;
	int		status;
	int		n_args;

	n_args = counter_args(tokens);
	if (n_args > 1)
		exit_error(data, ERR_EXIT_TOO_MANY, EXIT_USE);
	if (n_args = 1)
	{
		if (is_numeric(tokens[1].value))
		{
			num = ft_atol(tokens[1].value);
			status = num % 256;
			return (status);
		}
		exit_error(data, ERR_EXIT_NUMERIC, EXIT_USE);
	}
	return (0);
}

void my_exit(t_shell *data, t_token *tokens)
{
	int	status;

	status = check_exit(data, tokens);
	exit_succes(data, NULL, status);
}
