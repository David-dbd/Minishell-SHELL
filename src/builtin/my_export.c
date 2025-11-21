/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:22:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/21 14:31:18 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Implementación de la función `export` en un shell personalizado.
 *
 * Propósito:
 * Ejecutar asignaciones de variables de entorno cuando se llama al
 * comando `export` y mostrar las variables exportadas si
 * no se proporcionan argumentos válidos.
 *
 * Lógica general:
 * - La función recibe un puntero al token que representa el comando `export`.
 * - Comienza a recorrer los tokens a partir de su posición (`tokens->id`).
 * - En cada iteración:
 *   - Si encuentra un operador lógico (`AND`, `OR`), un pipe (`PIPE`) o un
 *     paréntesis de apertura (`PAREN_OPEN`),
 *     se detiene, ya que esos tokens indican un cambio de contexto en el
 *     árbol de ejecución.
 *   - Si el token actual es de tipo `ASIGNATION`, se ejecuta la asignación
 *     mediante `is_it_asig()`.
 *   - Los tokens que no sean asignaciones se ignoran silenciosamente.
 *
 * Comportamiento especial:
 * - Si no se ejecuta ninguna asignación (es decir, si `export` se llama
 *   sin argumentos válidos),
 *   se imprime la lista de variables exportadas actualmente en el entorno.
 * - Cada variable se muestra en formato Bash-compatible:
 *   `declare -x VAR="value"`.
 *
 * Ventajas del diseño:
 * - Evita ejecutar tokens irrelevantes o comandos que no forman
 *   parte de `export`.
 * - Respeta el flujo lógico del AST y la semántica real del comando `export`.
 * - Permite extender fácilmente el comportamiento para soportar más casos
 *   (como `export -p` o variables sin valor).
*/

static int	check_for_valid_args(t_token *tokens, int index)
{
	if (tokens[index].type == PIPE || tokens[index].type == AND
		|| tokens[index].type == OR || tokens[index].type == PAREN_OPEN
		|| is_redir_type(tokens[index].type))
		return (FALSE);
	return (TRUE);
}

static void	print_env_variables(t_env	*env)
{
	t_var	*var;

	var = env->vars;
	while (var)
	{
		if (var->type == ENV)
			ft_printf_fd(1, "declare -x %s=\"%s\"\n", var->key, var->value);
		if (var->type == EXP)
			ft_printf_fd(1, "declare -x %s\n", var->key);
		var = var->next;
	}
}

static int	check_wildcard_asig(t_shell *data, t_token *tokens, int i)
{
	int	result;

	result = 0;
	expand_wildcards(data, &data->prompt, tokens, FINAL_PHASE);
	result = check_asignation_syntax(&tokens[i], ENV);
	if (result)
		asignation(data, &tokens[i], ENV);
	else
	{
		result = check_asignation_syntax(&tokens[i], EXP);
		if (!result)
		{
			ft_printf_fd(STDERR, ERR_EXPORT, tokens[i].value);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

/*
	El caso edge supongo que sería que el hijo haga el export del temp_asig
	y que luego se llame por el padre a export, al nunca eliminar
*/

static int	asignation_type(t_shell *data, t_token *tokens, int i)
{
	if (tokens[i].type == ASIGNATION)
		asignation(data, &tokens[i], ENV);
	else if (tokens[i].type == WORD)
	{
		if (check_asignation_syntax(&tokens[i], EXP) == FALSE)
		{
			ft_printf_fd(STDERR, ERR_EXPORT, tokens[i].value);
			return (ERROR);
		}
		else
			asignation(data, &tokens[i], EXP);
	}
	else if (tokens[i].type == PLUS_ASIGNATION)
		asignation(data, &tokens[i], PLUS_ASIGNATION);
	else if (tokens[i].type == WILDCARD)
	{
		if (check_wildcard_asig(data, tokens, i) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

/*
	Auqnue se estan tratando las TEMMP_ASIGNATIONS como ASIGNATIONS, no hay
	riesgo de que se registre en el padre, ya que siempre se llamará a export
	con los arg_types correctos. Esto asegura que queden registrados solo en
	hijos cuando se llamen desde un fork (ya que nunca se llamará a export
	para)
*/

int	my_export(t_shell *data, t_token *tokens, t_env *env, t_node *node)
{
	int	i;
	int	exit_flag;

	exit_flag = 0;
	if (!node->arg_types)
	{
		print_env_variables(env);
		return (0);
	}
	i = 0;
	while (node->arg_types[i] != -1)
	{
		if (check_for_valid_args(tokens, node->arg_types[i]) == FALSE)
			break ;
		if ((is_asignation_type(tokens[node->arg_types[i]].type)
				|| tokens[node->arg_types[i]].type == WORD))
		{
			if (asignation_type(data, tokens, node->arg_types[i]) == ERROR)
				exit_flag = EXIT_FAIL;
		}
		i++;
	}
	return (exit_flag);
}
