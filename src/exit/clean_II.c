/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_II.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:53:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/28 16:43:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera un token en conreto que sea alojado dinamicamente
	se preve utilizar en los true o fake nodes.
*/

void	clean_token(t_token **token)
{
	if ((*token)->value)
	{
		free((*token)->value);
		(*token)->value = NULL;
	}
	if (*token)
	{
		free(*token);
		*token = NULL;
	}
}

/*
	Libera toda la memoria de una lista enlazada de redirecciones (t_redir)
	y establece el puntero de la lista a NULL.
*/

void	clean_redirs(t_redir **lst)
{
	t_redir	*current;
	t_redir	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*lst = NULL;
}

/*
	Libera toda la memoria de una lista enlazada a un nodo
	redirs, args y tokens falsos, luego setea el puntero a NULL
*/

void	clean_node(t_node **node)
{
	if ((*node)->args)
		ft_free_str_array((*node)->args);
	if ((*node)->arg_types)
		free((*node)->arg_types);
	if ((*node)->redir)
		clean_redirs(&(*node)->redir);
	if ((*node)->fake)
		clean_token(&(*node)->token);
	free(*node);
	*node = NULL;
}

/*
	Restaura los fd originales cuando hay redirecciones en el
	proceso del padre al ejecutarse builtins.
*/

void	restore_fd(t_exec *exec)
{
	dup2(exec->original_stdin, STDIN_FILENO);
	dup2(exec->original_stdout, STDOUT_FILENO);
}

/*
	Libera la memoria reservada para `prompt` y los nodos del `ast`
	tiene que llamarse en cada ciclo del minishell.
*/

void	clean_cycle(t_shell *data, t_prompt *prompt, t_node **ast_root)
{
	restore_fd(&data->exec);
	clean_prompt(prompt);
	clean_ast(*ast_root);
	*ast_root = NULL;
}
