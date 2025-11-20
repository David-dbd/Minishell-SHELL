/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_wildcards.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:18:03 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 23:54:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

static void	init_wildcard(t_shell *data, t_wild **wildcard)
{
	*wildcard = ft_calloc(1, sizeof(t_wild));
	if (!*wildcard)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static void	free_wildcard(t_wild **wildcard_ptr)
{
	t_wild	*wildcard;

	wildcard = *wildcard_ptr;
	if (!wildcard)
		return ;
	if (wildcard->key)
		free(wildcard->key);
	free (wildcard);
	*wildcard_ptr = NULL;
}

static int	extract(t_shell *data, t_token *token, t_wild *wildcard)
{
	if (token->value[0] == '.')
		wildcard->starts_with_dot = TRUE;
	else
		wildcard->starts_with_dot = FALSE;
	if (!extract_wildcard(data, token->value, &wildcard->key, &wildcard->type))
		return (FAILURE);
	return (SUCCESS);
}

static int	matches(t_shell *dat, t_token *token, t_wild *wildcard, int *n_dirs)
{
	*n_dirs = count_matches(dat, wildcard);
	if (*n_dirs == 0)
	{
		free_wildcard(&token->wildcard_info);
		return (FAILURE);
	}
	if (*n_dirs > 1 && token->id > 0 && is_redir_type(token[-1].type))
	{
		free_wildcard(&token->wildcard_info);
		syntax_error(dat, ERR_AMBIGUOUS_REDIR, EXIT_FAILURE);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	process_wildcard(t_shell *data, t_token *token)
{
	char	**new_tokens;
	int		n_dirs;

	new_tokens = NULL;
	n_dirs = 0;
	if (token->double_quoted)
		return (FAILURE);
	init_wildcard(data, &token->wildcard_info);
	if (!extract(data, token, token->wildcard_info))
		return (FAILURE);
	if (!matches(data, token, token->wildcard_info, &n_dirs))
		return (FAILURE);
	new_tokens = find_matches(data, token->wildcard_info, n_dirs);
	if (!new_tokens)
	{
		free_wildcard(&token->wildcard_info);
		return (FAILURE);
	}
	free_wildcard(&token->wildcard_info);
	rebuild_tokens(data, token, new_tokens, n_dirs);
	ft_free_str_array(&new_tokens);
	return (SUCCESS);
}
