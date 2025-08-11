/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_parenten.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:54:33 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_parenten(t_token *tokens, char c)
{
	if (c == '(')
		add_token(tokens, "(", PAREN_OPEN);
	else if (c == ')')
		add_token(tokens, ")", PAREN_CLOSE);
}
