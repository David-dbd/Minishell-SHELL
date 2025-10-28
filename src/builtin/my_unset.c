/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:56:06 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/28 11:22:20 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * my_unset - Elimina variables de entorno según los tokens proporcionados.
 *
 * @data: Puntero a la estructura principal del shell.
 * @env: Puntero a la estructura que contiene las variables de entorno.
 * @tokens: Array de tokens que pueden representar claves a eliminar.
 *
 * Esta función recibe args que busca en la lista doblemente enlazada de
 * variables de entorno. Si encuentra una coincidencia con la clave del token,
 * elimina el nodo correspondiente, actualizando los punteros prev y next
 * para mantener la integridad de la lista.
 *
 * Si el nodo a eliminar es el primero, actualiza env->vars.
 * Si es el único nodo, también actualiza env->vars y termina la función.
 * La memoria del nodo se libera mediante aux_delete().
*/

/*
	El primer caracter tiene que ser letra/_, mientras que el resto puede ser
	numeros y/o letras y _.
*/

static int	arg_syntax(char *arg)
{
	int	i;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (ERROR);
	i = 1;
	while (arg[i] != '\0')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	my_unset(t_shell *data, t_env *env, char **args)
{
	int	i;
	int	exit_flag;

	i = 0;
	exit_flag = 0;
	if (!args || !args[0])
		return (0);
	while (args[i] != NULL)
	{
		if (arg_syntax(args[i]) == ERROR)
		{
			ft_printf_fd(STDERR, ERR_UNSET, args[i]);
			exit_flag = EXIT_FAIL;
		}
		delete_var(env, args[i]);
		i++;
	}
	return (exit_flag);
}

/*
	Función para limpiar temp_asig. No creo que deba de discriminar entre los
	tokens que ya se limpiaron en los child o no, ya que si fue de ese modo,
	entonces simplemente no se encontrará la key en el env y seguira con el
	siguiente token temp_asig hasta encontrar una key, la cual solo habrá
	si fue ejecutado en el padre.

	El primer loop elimina los tokens aunque en este punto no es de mucha
	relevancia para ser honesto, pero creo que es importante seguir los
	formalismos.

	El segundo loop elimina las variables de los tokens TEMP de la lista
	enlazada, lo cual es MUY importante para poder seguir un flujo limpio y
	lógico que no afecte el shell padre/global
*/

void	my_clean_unset(t_shell *data, t_env *env, t_token *tokens, int *index)
{
	char	*char_to_find;
	char	*ptr;
	int		len;
	int		i;

	i = 0;
	char_to_find = NULL;
	if (!index || index[0] == -1)
		return ;
	while (index[i] != -1)
	{
		if (is_real_assignation_type(tokens[index[i]].type)
			|| tokens[index[i]].type == WORD)
		{
			i++;
			continue ;
		}
		//printf("It gets in the loop in my_clena_unset\n\n");
		//printf("%s\n\n", tokens[index[i]].value);
		ptr = ft_calloc(ft_strlen(tokens[index[i]].value) + 1, sizeof(char));
		if (!ptr)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		if (tokens[index[i]].type == TEMP_ASIGNATION)
			char_to_find = ft_strchr(tokens[index[i]].value, '=');
		else if (tokens[index[i]].type == TEMP_PLUS_ASIGNATION)
			char_to_find = ft_strchr(tokens[index[i]].value, '+');
		if (char_to_find)
			len = char_to_find - tokens[index[i]].value;
		ft_memcpy(ptr, tokens[index[i]].value, len);
		delete_var(env, ptr);
		free (ptr);
		ptr = NULL;
		char_to_find = NULL;
		i++;
	}
}
