/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:22:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/03 22:43:59 by migarrid         ###   ########.fr       */
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

static int	check_for_valid_args(t_token *tokens, int i)
{
	if (tokens[i].type == PIPE || tokens[i].type == AND
		|| tokens[i].type == OR || tokens[i].type == PAREN_OPEN)
		return (FALSE);
	return (TRUE);
}

static void	print_env_variables(t_var	*var)
{
	while (var)
	{
		if (var->type == ENV)
			printf("declare -x %s=\"%s\"\n", var->key, var->value);
		if (var->type == EXP)
			printf("declare -x %s\n", var->key);
		var = var->next;
	}
}

static int	asignation_type(t_shell *data, t_token *token, t_env *env)
{
	if (token->type == ASIGNATION)
		asignation(data, token, ENV);
	else if (token->type == WORD)
	{
		if (check_asignation_syntax(token, EXP) == FALSE)
		{
			ft_printf_fd(STDERR, ERR_EXPORT, token->value);
			return (EXIT_FAIL);
		}
		else
			asignation(data, token, EXP);
	}
	else if (token->type == PLUS_ASIGNATION)
		asignation(data, token, PLUS_ASIGNATION);
	return (SUCCESS);
}

/*
	1. Si solo es export export, entonces deberia de exportarla. Sin embargo, si
	fuese export a=1 export b=3 entonces solo deberia de exportar a y b no la
	palabra.
	**También pasa lo mismo con las locales. Si es a=1 export b=2 el export es
	built-in

	**2. this should be a error: e=1 hola r=4 as well as e=1 hola
	**eliminate the token in check_externs when ignore so that the error appears

	**3. Give the expansion bool if not already set up
*/
int	my_export(t_shell *data, t_token *tokens, t_token *token, t_env *env)
{
	t_var	*var;
	int		i;
	int		result;
	int		args_found;

	var = env->vars;
	i = token->id + 1;
	args_found = FALSE;
	while (i < data->prompt.n_tokens)
	{
		if (i > 0)
			args_found = TRUE;
		if (check_for_valid_args(tokens, i) == FALSE)
			break ;
		if ((is_asignation_type(tokens[i].type) || tokens[i].type == WORD)
			&& tokens[i].type != BUILT_IN)
			result = asignation_type(data, &tokens[i], env);
		i++;
	}
	if (!args_found)
		print_env_variables(var);
	return (result);
}
