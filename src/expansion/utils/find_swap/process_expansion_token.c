/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_expansion_token.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:53:02 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 16:54:37 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	process_expansion_token(t_shell *data, t_token *token, int phase)
{
	char	*key_to_find;
	int		number_of_dollars;
	int		found;

	found = FALSE;
	aux_mem_alloc(data, token, &key_to_find);
	number_of_dollars = get_symbol_to_expand_count(token->value);
	while (number_of_dollars > 0)
	{
		found = extract_key(data, token, &key_to_find, phase);
		number_of_dollars--;
	}
	free(key_to_find);
	key_to_find = NULL;
	return (found);
}
