/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_wildcards.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:18:03 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 01:03:27 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"


static int	extract(t_shell *d, t_token *t, int *wildc_type, char **key_to_find)
{
	if (!extract_wildcard(d, t->value, key_to_find, wildc_type))
	{
		if (*key_to_find)
			free(*key_to_find);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	matches(t_shell *d, char **key_to_find, int wildc_type, int *n_dirs)
{

	*n_dirs = count_matches(d, *key_to_find, wildc_type);
	// printf("matches: %d\n\n", *n_dirs);
	if (*n_dirs == 0)
	{
		free(*key_to_find);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	process_wildcard(t_shell *data, t_token *token)
{
	char	**new_tokens;
	char	*key_to_find;
	int		n_dirs;
	int		wildcard_type;

	new_tokens = NULL;
	key_to_find = NULL;
	wildcard_type = 0;
	n_dirs = 0;
	if (!extract(data, token, &wildcard_type, &key_to_find))
		return (FAILURE);
	if (!matches(data, &key_to_find, wildcard_type, &n_dirs))
		return (FAILURE);
	new_tokens = find_matches(data, key_to_find, n_dirs, wildcard_type);
	if (!new_tokens)
	{
		free (key_to_find);
		return (FAILURE);
	}
	free (key_to_find);
	rebuild_tokens(data, token, new_tokens, n_dirs);
	ft_free_str_array(new_tokens);
	return (SUCCESS);
}
