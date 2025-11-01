/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:12 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/29 19:27:38 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	`exec_builtin` gestiona la ejecución de los comandos internos (builtins)
	del shell, como `echo`, `cd`, `export`, `env`, `unset`, `exit`,
	o asignaciones de variables.

	El comportamiento varía según el contexto del nodo y el modo de ejecución:

	1. **Ejecución en background (`&`)**:
	   Si el nodo está marcado como background, se crea un nuevo proceso con
	   `fork()`. El hijo ejecuta el builtin correspondiente mediante
	   `which_builtin()`, mientras que el padre imprime el PID del proceso
	   en segundo plano. En este caso, el código de salida se restablece
	   a 0, ya que el fork fue exitoso, incluso si el comando interno falló.

	2. **Ejecución normal (foreground)**:
	   Si no está en segundo plano, el builtin se ejecuta directamente en
	   el proceso actual, sin crear un nuevo `fork()`. Esto es necesario
	   para comandos que modifican el entorno del shell (`cd`, `export`,
	   `unset`, etc.), ya que los cambios deben persistir en el proceso
	   principal.

	3. **Modo CHILD (subshell o pipeline)**:
	   Si el builtin se ejecuta dentro de un proceso hijo (por ejemplo,
	   parte de una subshell o un pipe), el proceso finaliza con el código
	    de salida del builtin mediante `exit_error()`.

	Esta estrategia equilibra correctamente el manejo de entorno, concurrencia
	y control de procesos, garantizando el comportamiento esperado tanto en
	ejecución secuencial como en estructuras complejas.
*/

void	exec_builtin(t_shell *data, t_node *node, t_exec *exec, int mode)
{
	pid_t	pid;

	if (mode == FATHER)
		expansion_final_process(data, node);
	if (node->background) //si es por background
	{
		pid = fork();
		if (pid == ERROR)
			exit_error(data, ERR_FORK, FAIL);
		if (pid == 0)
		{
			setup_signals_child();
			apply_properties(data, node, exec->env, CHILD);
			which_builtin(data, node->token, node);
			exit_succes(data, NULL, data->exit_code);
		}
		ft_printf_fd(STDOUT, "[&] %d\n", pid);
		data->exit_code = OK; //da 0 porque el fork en si fue exitoso
		return ;
	}
	if (apply_properties(data, node, exec->env, mode) == SUCCESS)
		which_builtin(data, node->token, node);
	if (mode == CHILD) // si era child de un pipe se sale con su exit_code
		exit_succes(data, NULL, data->exit_code);
	return ;
}
/*
static void     eliminate_arg(char **args, int index)
{
   int i;


   if (!args || !args[index])
       return ;
   free (args[index]);
   i = index;
   while (args[i])
   {
       args[i] = args[i + 1];
       i++;
   }
   args[i] = NULL;
}


static int arg_exists_in_tokens(char *arg, t_token *tokens, int start, int n_tokens)
{
   int j;


   j = start;
   while (j < n_tokens)
   {
       if (tokens[j].value && ft_strnstr(tokens[j].value, arg, ft_strlen(tokens[j].value)))
           return (true);
       j++;
   }
   return (false);
}


static void compare_args_vs_tokens(t_token *tokens, t_node *node, int n_tokens)
{
   int i;


   i = 0;
   while (node->args[i])
   {
       if (!arg_exists_in_tokens(node->args[i], tokens, node->token->id + 1, n_tokens))
       {
           eliminate_arg(node->args, i);
           continue; // no incrementamos i porque los args se han corrido
       }
       i++;
   }
}


static int check_for_expansion(t_token *tokens, int id, int n_tokens)
{
   id += 1;
   while (id < n_tokens)
   {
       if (tokens[id].type == EXPANSION)
           return (TRUE);
       if (is_delimiter_type(tokens[id].type) || is_cmd_builtin_type(tokens[id].type))
           break ;
   }
   return (FALSE);
}
*/
