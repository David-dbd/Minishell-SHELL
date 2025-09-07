/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_scape.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:25:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Procesa el carácter de escape '\'.
	- Si está al final del input, crea un token SCAPE.
	- Si sigue otro carácter, lo procesa como parte de una palabra.
*/

void	is_scape(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	if (str[*i] == '\\' && str[*i + 1] == '\0')
	{
		add_token(data, prompt, "\\", SCAPE);
		(*i)++;
	}
	else if (str[*i] == '\\' && str[*i + 1])
		is_word(data, prompt, str, i);
}
