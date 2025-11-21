/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_script_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:35:24 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 15:35:28 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_script_arg(t_shell *d, t_prompt *prom, const char *str, int *i)
{
	char	*token_to_ignore;

	token_to_ignore = ft_substr(str, *i - 1, 2);
	if (!token_to_ignore)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	add_token(d, prom, token_to_ignore, SCRIPT_ARG);
	(*i)++;
}
