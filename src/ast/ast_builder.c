/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:36:48 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:10:02 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

extern char	*g_type_names[];

/*
	🟢 1. Inicio del análisis
		La función principal del parser es ast_builder().
		Esta inicializa el índice en 0 y llama a parse_sequence(),
		también llamada parse_sequence(), que tiene menor precedencia.

	🟡 2. Nivel de punto y coma (;)
		parse_sequence() analiza si hay separadores como ; o &.
		Llama a parse_and_or() para construir el nodo izquierdo.
		Si encuentra un ;, agrupa con otro nodo derecho.
		Si no hay ;, devuelve el nodo izquierdo tal cual.

	🔵 3. Nivel de operadores lógicos (&&, ||)
		parse_and_or() llama a parse_pipe() para el nodo izquierdo.
		Si encuentra && o ||, agrupa con otro nodo derecho.
		 Si no hay operador lógico, devuelve el nodo izquierdo.

	🔴 4. Nivel de pipes (|)
		parse_pipe() llama a parse_parentheses() para el nodo izquierdo.
		Si encuentra un |, agrupa con otro nodo derecho.
		Si no hay pipe, devuelve el nodo izquierdo.

	⚪ 5. Nivel de paréntesis (( ))
		parse_parentheses() verifica si hay paréntesis de apertura.
		Si lo hay, avanza y llama a parse_sequence() internamente.
		Luego busca el paréntesis de cierre y lo valida.
		Devuelve un nodo PAREN con la subexpresión.
		Si no hay paréntesis, delega a parse_command().

	🟤 6. Nivel de comandos
		parse_command() agrupa palabras, variables y asignaciones.
		Recorre tokens tipo WORD como argumentos.
		Devuelve un nodo CMD con todos los elementos agrupados y redirs.

	🔁 7. Fin de la recursión
		Cada función superior recibe el nodo inferior construido.
		Si encuentra su operador, agrupa con otro nodo recursivo.
		Si no hay operador, devuelve el nodo tal cual.
		ast_builder() recibe el nodo raíz del árbol sintáctico.

	🧩 Esquema de llamadas
		Código
		ast_builder
		└── parse_sequence
			└── parse_and_or
				└── parse_pipe
					└── parse_parentheses
						└── parse_command

		Cada nivel llama al siguiente más fuerte para construir operandos.
		La recursión ocurre solo si hay operador que requiere agrupar.
*/

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
	printf(" [Args ");
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

	if (!index || !index[0])
		return;
	printf(" [tokens_index: ");
	i = 0;
	while (index[i])
	{
		printf("%d", index[i]);
		if (index[i + 1])
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
		return;
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
		return;

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
		return;
	}

	i = 0;
	while (i < 100)
		is_last_array[i++] = 0;

	printf("\n=== AST ===\n");
	print_node_helper(root, 0, is_last_array);
}


/*
 * Construye el Árbol de Sintaxis Abstracta (AST): crea nodo raíz SHELL
 * y parsea secuencias de tokens en subárboles izquierdo y derecho
*/

static void	test_built_in(t_shell *data, t_token *tokens, int n_tokens, t_node *node)
{
	int	i;

	i = 0;
	while (i < n_tokens)
	{
		if (tokens[i].type == BUILT_IN || is_asignation_type(tokens[i].type))
			which_builtin(data, &tokens[i], node);
		i++; //ESTO HACE QUE SE MULTIPLIQUEN LOS RESULTADOS x2
	}
}

void	ast_builder(t_shell *data, t_token *tokens, int n_tokens)
{
	int		i;

	i = 0;
	if (n_tokens == 0)
		return ;
	data->ast_root = parse_sequence(data, tokens, &i, n_tokens);
	print_ast(data->ast_root);
	//test_built_in(data, tokens, n_tokens, data->ast_root);
}
