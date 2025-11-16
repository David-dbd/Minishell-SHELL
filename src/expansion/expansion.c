/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:57:33 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/16 15:34:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Sistema de expansión de variables en un shell personalizado.
 *
 * Este módulo permite detectar, validar y reemplazar variables de entorno
 * dentro de los tokens que contienen expresiones como `$VAR`.
 *
 * Flujo general:
 * 1. La función `send_tokens_for_expansion` recorre todos los tokens del
 *    prompt. Si un token está marcado como tipo `EXPANSION`,
 *    se envía a la función  `expansion`.
 *
 *
 * 2. En `expansion`, se reserva memoria para almacenar el nombre de la
 *    variable (`key_to_find`) y se cuenta cuántos signos `$` hay
 *    en el token con `count_char`.
 *    Esto permite manejar múltiples variables dentro de una misma cadena.
 *
 * 3. Por cada `$` detectado, se llama a `extract_key_string`, que:

 *    - Usa `copy_key` para extraer el nombre de la variable si es
 *     válida (letras sin espacios). Llama a `find_key_in_lst` para buscar
 *     el valor correspondiente en la lista `env.vars`.

 *    - Si no se encuentra y estamos en `phase == 2`, se reemplaza
 *     el token por una cadena vacía.
 *
 * 4. Si se encuentra la variable, `copy_value` se encarga de:
 *    - Calcular el nuevo tamaño del token con `calculate_total_lenght`.
 *    - Reasignar memoria con `ft_realloc`.
 *    - Localizar el `$VAR` en el token con `find_key_in_new_buffer`.
 *    - Reemplazar el nombre de la variable por su valor.
 *
 * 5. Funciones auxiliares:
 *    - `aux_mem_alloc`: reserva memoria para `key_to_find`.
 *    - `count_char`: cuenta cuántos signos `$` hay en una cadena.
 *    - `calculate_total_lenght`: calcula el tamaño necesario para
 *       el token expandido.
 *    - `find_key_in_new_buffer`: localiza la posición del `$VAR` a reemplazar.
 *
 * Validaciones:
 * - Solo se consideran válidas las variables que comienzan con `$`
 *    seguido de letras.
 * - Se evita copiar variables mal formadas como `$ VAR` o `$1`.
 * - Se maneja correctamente el error de memoria con `exit_error`.
 *
 * Este sistema permite una expansión segura, dinámica y escalable de variables
 * dentro de un shell, respetando fases de ejecución y tipos de token.
*/

static int	aux_mem_alloc(t_shell *data, t_token *token, char **key_to_find)
{
	int	len;

	len = ft_strlen(token->value);
	*key_to_find = ft_calloc((len * 2) + 1, sizeof(char));
	if (!*key_to_find)
		exit_error(data, ERR_MALLOC, EXIT_USE);
	return (SUCCESS);
}

static int	tokens_size_has_changed(int *original_size, int new_size)
{
	if (*original_size > new_size)
	{
		*original_size = new_size;
		return (TRUE);
	}
	return (FALSE);
}

static int	get_symbol_to_expand_count(char *str)
{
	int	number_of_symbols;

	number_of_symbols = ft_count_char(str, '$');
	number_of_symbols += ft_count_char(str, '~');
	return (number_of_symbols);
}

static int	process_expansion_token(t_shell *data, t_token *token, int phase)
{
	char	*key_to_find;
	int		number_of_dollars;
	int		found;

	found = FALSE;
	aux_mem_alloc(data, token, &key_to_find);
	number_of_dollars = get_symbol_to_expand_count(token->value);
	while (number_of_dollars > 0)
	{
		found = extract_key(data, token, &key_to_find, phase);
		number_of_dollars--;
	}
	free(key_to_find);
	key_to_find = NULL;
	return (found);
}

int expansion(t_shell *data, t_token *tokens, int i, int phase)
{
	int	found;
	int	original_size;

	found = FALSE;
	original_size = data->prompt.n_tokens; //para evitar omitir un token cuando se elimina en expand_empty_str porque hace tokens--; en eliminate_token
	while (i < data->prompt.n_tokens)
	{
		if (phase == FINAL_PHASE && is_delimiter_type(tokens[i].type))
			return (SUCCESS);
		if (tokens[i].type == EXPANSION)
		{
			if ((i > 1 && tokens[i - 1].type == REDIR_HEREDOC) || (i >= 2 && is_quote_type(tokens[i - 1].type) && tokens[i - 2].type == REDIR_HEREDOC))
			{
				i++;
				continue ;
			}
			found = process_expansion_token(data, &tokens[i], phase);
			if (found == ERROR)
				return (clean_cycle(&data->exec, &data->prompt, NULL), found);
		}
		if (tokens_size_has_changed(&original_size, data->prompt.n_tokens))
			continue ;
		i++;
	}
	return (found);
}
