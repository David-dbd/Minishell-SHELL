/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_wildcards.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:18:03 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/01 18:42:23 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

void	process_wildcard(t_shell *data, t_token *token)
{
	char	**new_tokens;
	char	*key_to_find;
	int		n_dirs;

	if (!extract_wildcard(data, token->value, &key_to_find))
	{
		if (key_to_find)
			free(key_to_find);
		return ;
	}
	n_dirs = count_matches(data, key_to_find);
	if (n_dirs == 0)
	{
		free(key_to_find);
		return ;
	}
	new_tokens = find_matches(data, key_to_find, n_dirs);
	if (!new_tokens)
	{
		free(key_to_find);
		return ;
	}
	rebuild_tokens(data, token, new_tokens, n_dirs);
	ft_free_str_array(new_tokens);
}
