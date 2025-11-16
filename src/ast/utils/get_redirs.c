/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 20:59:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/16 21:07:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	set_index(t_token *tokens, int *j, int *i)
{
	if (*i >= 2 && is_redir_type(tokens[*i - 2].type))
		*j = *i - 2;
	else
		*j = *i;
}

/*
 * Encuentra el último nodo en lista de redirecciones:
 * recorre la lista hasta el final para añadir nuevos elementos
*/

t_redir	*lstlast_redir(t_redir *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

bool expand_heredoc(t_token *token)
{
	if (!token[+1].type)
		return false;
	return !(token[+1].single_quoted || token[+1].double_quoted);
}

/*
 * Añade redirección a lista: crea estructura con tipo, archivo
 * y descriptor, manejando números explícitos o valores por defecto
*/

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
		redir->fd_redir = token->value[0] - '0';
	else
	{
		if (redir->type == REDIR_INPUT)
			redir->fd_redir = STDIN_FILENO;
		else
			redir->fd_redir = STDOUT_FILENO;
	}
	if (redir->type == REDIR_HEREDOC)
		get_heredoc(data, redir, filename, expand_heredoc(token));
	if (!lst)
		return (redir);
	last = lstlast_redir(lst);
	last->next = redir;
	return (lst);
}

/*
 * Obtiene lista de redirecciones desde tokens: recorre tokens
 * recolectando operadores de redirección y sus archivos asociados
*/

t_redir	*get_redirs(t_shell *data, t_token *tokens, int *i, int mode)
{
	int		j;
	t_redir	*head;

	head = NULL;
	set_index(tokens, &j, i);
	while (j < data->prompt.n_tokens && !is_delimiter_type(tokens[j].type))
	{
		if (is_redir_type(tokens[j].type))
		{
			head = add_redir(data, head, &tokens[j], tokens[j + 1].value);
		}
		j++;
	}
	if (mode == SUBSHELL || mode == TRUE)
		*i = j;
	return (head);
}
