/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:59:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 22:44:57 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_redir	*lstlast_redir(t_redir *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_redir	*add_redir(t_shell *data, t_redir *lst, t_token *token, char *filename)
{
	t_redir	*redir;
	t_redir	*last;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	redir->type = token->type;
	redir->filename = filename;
	if (ft_isdigit(token->value[0]))
		redir->fd = token->value[0] - '0';
	else
	{
		if (redir->type == REDIR_INPUT)
			redir->fd = 0;
		else
			redir->fd = 1;
	}
	if (!lst)
		return (redir);
	last = lstlast_redir(lst);
	last->next = redir;
	return (lst);
}

t_redir	*get_redirs(t_shell *data, t_token *tokens, int mode, int *i)
{
	int		j;
	t_redir	*head;

	head = NULL;
	j = *i;
	while (j < data->prompt.n_tokens && !is_delimiter_type(tokens[j].type))
	{
		if (is_redir_type(tokens[j].type))
			head = add_redir(data, head, &tokens[j], tokens[j + 1].value);
		j++;
	}
	if (mode == SUBSHELL)
		*i = j;
	return (head);
}
