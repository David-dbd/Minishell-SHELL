/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_and.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:44:05 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:00:25 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Procesa el carácter '&' en el input: si está seguido de otro '&',
	crea un token de tipo AND; si va seguido de un dígito, crea un
	token WORD con el descriptor; si no, crea un token de tipo BACKGROUND.
	Actualiza el índice para saltar los caracteres ya procesados.
*/

void	is_and(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	char	*dup_fd;

	if (str[*i] == '&')
	{
		if (str[*i + 1] == '&')
		{
			add_token(data, prompt, "&&", AND);
			(*i) += 2;
		}
		else if (ft_isdigit(str[*i + 1]))
		{
			dup_fd = ft_substr(str, *i, 2);
			if (!dup_fd)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			add_token(data, prompt, dup_fd, WORD);
			(*i) += 2;
		}
		else
		{
			add_token(data, prompt, "&", BACKGROUND);
			(*i)++;
		}
	}
}
