/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_final_process.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:04:23 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 08:58:21 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Función para generar un arr de types. Ese arr contiene información vital
	que se usara para después de la expansión.
	Se libera prompt->before_tokens_type en cada iteración porque los tokens
	pueden ir cambiando a medida que se expande y simplifica
*/

void	create_before_tokens(t_shell *d, t_token *tokens, t_prompt *prompt)
{
	int	i;
	int	*token_type_buffer;

	i = 0;
	if (prompt->before_tokens_type)
	{
		free (prompt->before_tokens_type);
		prompt->before_tokens_type = NULL;
	}
	token_type_buffer = ft_calloc(prompt->n_tokens + 1, sizeof(int));
	if (!token_type_buffer)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	while (i < prompt->n_tokens)
	{
		token_type_buffer[i] = tokens[i].type;
		i++;
	}
	prompt->before_tokens_type = token_type_buffer;
}

/*
	Cambiar los tokens NO_SPACE invalidos por tokens INDIFIRENT para que
	simplify_tokens no los elimine y no junte los tokens.

	Ejemplos:
			echo $USE" "hello" ".txt
			-> CMD/EXPANSION/NO_SPACE/WORD/NO_SPACE/WORD
			-> CMD/NO_SPACE/WORD/NO_SPACE/WORD
			-> CMD/INDIFERENT/WORD/NO_SPACE/WORD
			-> CMD/INDIFERENT/WORD/WORD

			echo $USE.txt
			-> CMD/EXPANSION/NO_SPACE/WORD
			-> CMD/NO_SPACE/WORD
			-> CMD/INDIFERENT/WORD

			echo" "$USE.txt
			-> CMD/NO_SPACE/EXPANSION/NO_SPACE/WORD
			-> CMD/NO_SPACE/NO_SPACE/WORD
			-> CMD/WORD

	Esta escrito de esta manera para que visualmente sea totalmente claro
	para cualquiera.
*/

static int	check_cases(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (i > 0 && (i + 1) < prompt->n_tokens)
	{
		if (is_cmd_builtin_type(tokens[i - 1].type)
			&& tokens[i + 1].type != NO_SPACE)
		{
			tokens[i].type = DONT_ELIMINATE;
		}
		else if (tokens[i - 1].type == WORD
			&& tokens[i + 1].type == WORD)
		{
			tokens[i].type = DONT_ELIMINATE;
		}
		else if (tokens[i - 1].type == NO_SPACE
			|| tokens[i + 1].type == NO_SPACE)
		{
			eliminate_token(data, prompt, tokens, i);
			reconect_nodes_tokens(data, data->ast_root, prompt->tokens);
			return (1);
		}
	}
	return (0);
}

static void	prepare_simplify(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	*tokens_before;

	i = 0;
	tokens_before = prompt->before_tokens_type;
	while (i < prompt->n_tokens)
	{
		if (tokens_before[i] == EXPANSION && tokens[i].type == NO_SPACE)
		{
			if (check_cases(data, prompt, tokens, i))
				continue ;
		}
		i++;
	}
}

static int	if_theres_an_expan(t_token *start_t, t_token *tokens, t_prompt *p)
{
	int	i;

	i = 0;
	while (i < p->n_tokens && &tokens[i] != start_t) //probar con hash en lugar de esto
		i++;
	while (i < p->n_tokens)
	{
		if (is_delimiter_type(tokens[i].type))
			return (FALSE);
		if (tokens[i].type == EXPANSION)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

/*

	Se expande solo los args del nodo ya que si intentamos expandir todos los
	tokens en FINAL_PHASE, luego, si en otro nodo hay un valor que dependia
	de otro, entonces se queda vacio. Es por esto que no se puede dejar que
	la expansión de un nodo afecte a los otros.

	Luego se usa un prepare_last_simplify para casos edge en los que al no
	poder expandir, el token queda vacio, y, si hubiese un NO_SPACE delante,
	este podría hacer que el cmd y otro token se unan aunque no se quiera.
	Es por esto que se tienen que vigilar estos casos.

	Luego se usa simplify_tokens para poder eliminar NO_SPACE tokens y juntar
	en caso de que la expansión haya sido exitosa y requiera juntarse con un
	token WORD o algo por el estilo...

	Para poder recuperar la conexicón con el token cmd, se aplica un sistema
	de hash único para cada token, al igual que su id (token->id, que
	representa su index en el arr), pero este también lo tendrá el nodo,
	de modo que solo hay que hacer un match de hashes para encontrar el token
	después de que simplify haya eliminado y reorganizado los tokens.

	Luego se verifica si hubo alguna expansión. Si es así, entonces se
	ajustan los args con get_args_for_binary.

	split_expansion_result sirve para poder hacer un split de el resultado
	de expandir una clave que como valor tenga "echo -n", de modo que se
	separan los tokens y se ejecutan. De lo contrario se ejecutarian como un
	soolo token echo -n.
	split_expansion_result tambien vuelve a reorganizar los tokens, de modo
	que se vuelve a perder la referencia con los nodos y por eso hay que
	llamara reconect una vez mas.

	En conclusión, cada vez que se alteran los tokens en esta etapa, siempre
	se perderá su referencia con los nodos y por eso hay que reconectarlos
	a través del hash. Esta solución nos da bastante flexibilidad a la hora
	de poder seguir transformando los tokens durante la ejecución sin ningun
	problema de estados, que es el problema mas grande en esta etapa.

	De este modo:
	* No se tiene que hacer una expansión extra a otro tipo de dato
	que no sea tokens, como char **.
	* Se sigue el flujo inicialmente pensado.
	* No se complica más la lógica.
	* No se afecta a ninguna parte/proceso del proyecto.
	* Se puede seguir contando con todos los datos que hemos recavado
	para poder seguir los procesos con afinidad.
*/

int expansion_final_process(t_shell *data, t_node *node)
{
	int	i;

	i = 0;
	if (if_theres_an_expan(node->token, data->prompt.tokens, &data->prompt))
	{
		create_before_tokens(data, data->prompt.tokens, &data->prompt);
		expansion(data, data->prompt.tokens, node->token->id, FINAL_PHASE);
		if (data->prompt.n_tokens == 0)
			return (FAILURE);
		 print_tokens_debug(&data->prompt);
		prepare_simplify(data, &data->prompt, data->prompt.tokens);
		 print_tokens_debug(&data->prompt);
		simplify_tokens(data, &data->prompt, data->prompt.tokens);
		reconect_nodes_tokens(data, data->ast_root, data->prompt.tokens);
		split_expansion_result(data, &data->prompt, data->prompt.tokens);
		reconect_nodes_tokens(data, data->ast_root, data->prompt.tokens);
		i = node->token->id;
		if (node->args)
			ft_free_str_array(&node->args);
		expand_wildcards(data, &data->prompt, data->prompt.tokens, FINAL_PHASE); //Las wildcards que no se hayan expandido llegado este punto es debido a que dependen de una expansion que no se ha podido hacer
		node->args = get_args_for_binary(data, data->prompt.tokens, &i);
	}
	return (SUCCESS);
}
