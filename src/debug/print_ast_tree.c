/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:51:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 21:56:16 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_indent(int depth, int *is_last_array)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		if (i == depth - 1)
			printf(is_last_array[i] ? "└─ " : "├─ ");
		else
			printf(is_last_array[i] ? "   " : "│  ");
		i++;
	}
}

static void	print_args(char **args)
{
	int	i;

	if (!args || !args[0])
		return;
	printf(" [Args: ");
	i = 0;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

static void	print_token_index(int *index)
{
	int	i;

	if (!index)
		return;

	printf(" [Index: ");
	i = 0;
	while (index[i])
	{
		printf("%d", index[i]);
		if (index[i + 1] != 0)
			printf(", ");
		i++;
	}
	printf("]");
}


static void	print_redirs(t_redir *redirs)
{
	t_redir	*current;
	int		first;

	if (!redirs)
		return ;
	printf(" {");
	current = redirs;
	first = 1;
	while (current)
	{
		if (!first)
			printf(", ");
		if (current->fd >= 0)
			printf("%s %s (fd:%d)", g_type_names[current->type],
				current->filename, current->fd);
		else
			printf("%s %s", g_type_names[current->type], current->filename);
		first = 0;
		current = current->next;
	}
	printf("}");
}

static void	print_node_helper(t_node *node, int depth, int *is_last_array)
{
	const char	*value;

	if (!node)
		return ;

	// Imprimir indentación
	if (depth > 0)
		print_indent(depth, is_last_array);

	// Imprimir nodo
	value = (node->token && node->token->value) ? node->token->value : NULL;
	if (value)
		printf("%s: \"%s\"", g_type_names[node->type], value);
	else
		printf("%s", g_type_names[node->type]);

	// Imprimir argumentos si existen
	print_args(node->args);
	print_token_index(node->arg_types);

	if (node->background)
		printf(" <Background> ");

	// Imprimir redirecciones si existen
	print_redirs(node->redir);
	printf("\n");

	// Imprimir hijos
	if (node->left)
	{
		is_last_array[depth] = !node->right;
		print_node_helper(node->left, depth + 1, is_last_array);
	}
	if (node->right)
	{
		is_last_array[depth] = 1;
		print_node_helper(node->right, depth + 1, is_last_array);
	}
}

void	print_ast(t_node *root)
{
	int	is_last_array[100];
	int	i;

	if (!root)
	{
		printf("(empty)\n");
		return ;
	}
	i = 0;
	while (i < 100)
		is_last_array[i++] = 0;
	printf("\n=== AST ===\n");
	print_node_helper(root, 0, is_last_array);
}
