/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 01:44:27 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/17 15:47:16 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	get_arg_types() sirve para contextualizar los argumentos de un comando.
	En especial, se usa para `export`, que puede manejar ASIGNATIONS,
	PLUS_ASIGNATIONS (`+=`) y WORD.

	Ejemplos:
	*export [args: var=1, var+=1, hola]-> [context: ASIGNATION, PLUS_ASIG, WORD]

	*var+=1 export [args: var=1, var+=1, hola]->
	[context: PLUS_ASIG, ASIGNATION, PLUS_ASIG, WORD]

	*var=1 ls [args: -l] [temp: var=1] ->[context: TEMP_ASIGNATION]

	En vez de guardar los tipos (`t_type`), se guarda el índice del token
	original. Esto permite acceder a sus vecinos y analizar el contexto,
	como en `is_it_env_or_local()`.

	Si hay asignaciones temporales (`TEMP_ASIGNATION`), también se incluyen
	en el array, ya que `export` las tratará como asignaciones normales
	en el proceso hijo.

	Esto evita duplicar trabajo y permite que el executor tenga toda la
	información necesaria sin reanalizar los argumentos.

	En get_bin_args(); se avanza el index desde el primer argumento del cmd
	hasta el último. Aquí, start_i hace refernecia a la posición de i
	sobre el cmd otra vez, mientras que end_j hace referencia al index en el
	úlrimo cmd, de modo que se puede cubrir la distancia exacta entre el cmd
	y su ultimo argumento

	Si arg_index no se movio es porque no hay args o no son validos
*/

static int	create_dinamic_arr(t_shell *data, int **arg_types, int i, int j)
{
	*arg_types = (int *)malloc((j - i) * sizeof(int));
	if (!*arg_types)
	{
		data->error_state = TRUE;
		ft_printf_fd(STDERR, ERR_MALLOC);
		return (FAILURE);
	}
	ft_memset(*arg_types, -1, (j - i) * sizeof(int));
	return (SUCCESS);
}

static int	*alloc_arg_types(t_shell *dat, t_node *node, int start_i, int end_j)
{
	int	*arg_types;
	int	tmp_counter;
	int	tmp_token_index;
	int	len;

	tmp_counter = 0;
	if (node->assig_tmp)
	{
		while (node->assig_tmp[tmp_counter])
			tmp_counter++;
		if (!create_dinamic_arr(dat, &arg_types, start_i, (end_j + tmp_counter)))
			return (NULL);
		len = tmp_counter;
		//printf("%d", tmp_counter);
		tmp_token_index = tmp_counter;
		tmp_counter = 0;
		while (tmp_counter < len)//copiamos las temp_Asigs en el orden correcto (de la primera a la ultima)
		{
			arg_types[tmp_counter] = dat->prompt.tokens[start_i - tmp_token_index].id;
			//printf("token que se copia: %d\n\n", arg_types[tmp_counter]);
			tmp_counter++;
			tmp_token_index--;
		}
	}
	else
		if (!create_dinamic_arr(dat, &arg_types, start_i, end_j))
			return (NULL);
	return (arg_types);
}

static void	check_arg_index(int arg_index, int **arg_types)
{
	if (arg_index == 0)
	{
		//printf("zero\n\n");
		free (*arg_types);
		*arg_types = NULL;
	}
}

/*
	ignoro los tokens temp porque ya estan en el arr.
	Es muy importane la distinción entre malloc y calloc asi como el porque
	usar memset a -1.

	ft_calloc utiliza internamente memset o bzero y malloc, con lo cual son dos
	llamadas extra a otras funciones (3 en total), mientras que si llamo
	directamente a malloc aqui, solo es una llamada a una función.

	Utilizo memset ya que al utilizat calloc, el loop sigue contando porque
	en muchos casos 0 sera una posicoón valida, con lo cual estará incorrecto.

	De modo que si tengo var=1 ls -> [0, 0, 0 ...] o los 0 que sean.
	El punto es que todo eso es valido porque todas hacen parte de var=1

	En cambio, si utilizo memset a -1, puedo distiguir facilmente entre lo
	existente en el arr y lo que no existe aún, debido a que -1 ni siquiera
	es un index válido para un token.
*/

static int	find_correct_index(t_token *tokens, int *arg_types)
{
	int	i;

	i = 0;
	while (arg_types[i] != -1 && is_asignation_type(tokens[arg_types[i]].type))
		i++;
	return (i);
}

int	*get_arg_types(t_shell *data, t_node *node, int start_i, int end_j)
{
	t_token	*tokens;
	int		*arg_types;
	int		arg_index;

	tokens = data->prompt.tokens;
	//printf("get_arg_types: %s\n\n", data->prompt.tokens[start_i].value);
	arg_types = alloc_arg_types(data, node, start_i, end_j);
	if (!arg_types)
		return (NULL);
	if (!tokens[start_i].value || ft_strcmp(tokens[start_i].value, BUILTIN_EXPORT) != 0 && node->assig_tmp == NULL)
		return (free (arg_types), NULL);
	if (!tokens[start_i].value || ft_strcmp(tokens[start_i].value, BUILTIN_EXPORT) != 0 && tokens[start_i + 1].type)
		return (arg_types);
	if (start_i + 1 < data->prompt.n_tokens)
		start_i += 1;
	//printf("TOKENS:  %s\n\n", data->prompt.tokens[start_i].value);
	arg_index = find_correct_index(data->prompt.tokens, arg_types);
	while (start_i < data->prompt.n_tokens && start_i < end_j)
	{
		if (is_arg_type(tokens[start_i].type))
		{
			arg_types[arg_index] = tokens[start_i].id;
			arg_index++;
			// Si se simplican los tokens ya no son el id... hay un caso que no
			// funciona revisar --> 'ex'port A=1
			// A=1 es id 5 pero index 1
		}
		start_i++;
	}
	check_arg_index(arg_index, &arg_types);
	return (arg_types);
}
