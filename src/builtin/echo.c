/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:23:24 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 18:41:10 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_n_flag(char **args, int *new_line, int *i)
{
	int	j;

	j = 0;
	while (args[*i] && args[*i][0] == '-' && args[*i][1] == 'n')
	{
		j += 2;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] == '\0')
		{
			*new_line = FALSE;
			(*i)++;
		}
		else
			break ;
	}
}

void	shell_echo(char **args)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = TRUE;
	handle_n_flag(args[i], &new_line, &i);
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
}
