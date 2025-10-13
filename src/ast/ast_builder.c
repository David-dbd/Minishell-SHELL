#include "../../inc/minishell.h"

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
		Devuelve un nodo CMD con todos los elementos agrupados, redirs y mas.

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

void	ast_builder(t_shell *data, t_token *tokens, int n_tokens)
{
	t_node	*temp;
	int		i;

	i = 0;
	if (n_tokens == 0)
		return ;
	// print_tokens_debug(&data->prompt);
	data->ast_root = parse_sequence(data, tokens, &i, n_tokens);
	// print_ast(data->ast_root);
}
