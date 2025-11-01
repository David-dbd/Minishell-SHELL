/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_final_process.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:04:23 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 09:55:05 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Función para generar un arr de types. Ese arr contiene información vital
	que se usara para después de la expansión.
	Se libera prompt->before_tokens_type en cada iteración porque los tokens
	pueden ir cambiando a medida que se expande y simplifica
*/

static void	create_before_tokens(t_shell *d, t_token *tokens, t_prompt *prompt)
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
			if (i > 0 && (i + 1) < prompt->n_tokens)
			{
				if (is_cmd_builtin_type(tokens[i - 1].type)
				&& tokens[i + 1].type != NO_SPACE)
				{
					tokens[i].type = INDIFERENT;
				}
				else if (tokens[i - 1].type == WORD
					&& tokens[i + 1].type == WORD)
				{
					tokens[i].type = INDIFERENT;
				}
			}
		}
		i++;
	}
}

static void	reconect_nodes_tokens(t_shell *data, t_node *node, t_token *tokens)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (tokens[i].hash == node->token_hash)
		{
			node->token = &tokens[i];
			node->token->id = i;
			break ; //ya encontro el token correcto, así que hay que seguir con los demás
		}
		i++;
	}
	reconect_nodes_tokens(data, node->left, tokens);
	reconect_nodes_tokens(data, node->right, tokens);
}

static int if_theres_an_expansion(t_token *start_token, t_token *tokens, t_prompt *prompt)
{
	int i;

	i = 0;
	while (i < prompt->n_tokens && &tokens[i] != start_token)//probar con hash en lugar de esto
		i++;
	while (i < prompt->n_tokens)
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


	De este modo:
	* No se tiene que hacer una expansión extra a otro tipo de dato
	que no sea tokens, como char **.
	* Se sigue el flujo inicialmente pensado.
	* No se complica más la lógica.
	* No se afecta a ninguna parte/proceso del proyecto.
	* Se puede seguir contando con todos los datos que hemos recavado
	para poder seguir los procesos con afinidad.
*/

void expansion_final_process(t_shell *data, t_node *node)
{
	int	i;

	if (if_theres_an_expansion(node->token, data->prompt.tokens, &data->prompt))
	{
		create_before_tokens(data, data->prompt.tokens, &data->prompt);
		expansion(data, data->prompt.tokens, node->token->id, FINAL_PHASE);
		//printf("AFTER EXPANSION\n\n");
		print_tokens_debug(&data->prompt);
		prepare_simplify(data, &data->prompt, data->prompt.tokens);
		//printf("AFTER PREPARE_SIMPLY\n\n");
		//print_tokens_debug(&data->prompt);
		simplify_tokens(data, &data->prompt, data->prompt.tokens);
		//print_tokens_debug(&data->prompt);
		//printf("AFTER SIMPLIFY\n\n");
		reconect_nodes_tokens(data, data->ast_root, data->prompt.tokens);
		//print_tokens_debug(&data->prompt);
		i = node->token->id;
		if (node->args)
			free (node->args);
		node->args = get_args_for_binary(data, data->prompt.tokens, &i);
	}
	//print_ast(data->ast_root);
}
