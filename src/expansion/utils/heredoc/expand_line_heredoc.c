/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 17:38:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/16 21:06:33 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	aux_mem_alloc(t_shell *data, t_token *token, char *line, char **key_to_find)
{
	int	len;

	*token = (t_token){0};
	token->value = ft_strdup(line);
	token->heredoc = TRUE; // lo hago asi porque empty string sino reoganiza tokens y no deeberia pero la logica es compleja help david
	if (!token->value)
		exit_error(data, ERR_MALLOC, EXIT_USE);
	len = ft_strlen(token->value);
	*key_to_find = ft_calloc((len * 2) + 1, sizeof(char));
	if (!*key_to_find)
		exit_error(data, ERR_MALLOC, EXIT_USE);
	return (SUCCESS);
}

static int	get_symbol_to_expand_count(char *str)
{
	int	number_of_symbols;

	number_of_symbols = ft_count_char(str, '$');
	number_of_symbols += ft_count_char(str, '~');
	return (number_of_symbols);
}

int	expand_line_heredoc(t_shell *data, char **line)
{
	t_token	token;
	char	*key_to_find;
	int		number_of_dollars;
	int		found;

	found = FALSE;
	number_of_dollars = get_symbol_to_expand_count(*line);
	if (!number_of_dollars)
		return (FALSE);
	aux_mem_alloc(data, &token, *line, &key_to_find);
	while (number_of_dollars > 0)
	{
		found = extract_key(data, &token, &key_to_find, FINAL_PHASE);
		number_of_dollars--;
	}
	free(key_to_find);
	if (found)
	{
		free(*line);
		*line = token.value;
	}
	else
		free(token.value);
	key_to_find = NULL;
	return (found);
}
