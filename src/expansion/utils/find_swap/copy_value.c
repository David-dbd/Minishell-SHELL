/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:22:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 00:42:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Aquí se calcula el len_total. Este será el que nos indique el tamaño exacto
	y final que necesitamos para nuestro buffer.

	En caso de que el value este vacio (lo cual puede pasar), ft_strlen de value
	sería 0, con lo cual:

	total_len = len_of_token + len_of_key_value - len_of_key;
	sería en realidad:
	total_len = len_of_token - len_of_key;

	Ahora bien, esto en realidad no genera ningun problema debido a que
	len_of_key no tiene en cuenta el '$', solo el key, es decir, sobra 1 indice
	al final de la operación.

	Ejemplo:

	USER = ""
	"Hello $USER"

	len_of_key = 4; sin tener en cuenta el $. si lo tuviera, sería 5
	len_of_value_key = 0;
	token_len = 11;
	total_len = 11 + 0 - 4; -> 7

	De modo que el char 7 es: $
	Este char será reemplazado en expand(); por un vacio, de modo que no hay
	que distinguir si hay o no hay valor en value ya que implicitamente funciona
	Si por el contrario key_len si tuviese en cuenta el $ entonces si habria que
	hacer + 1 a total_len.

	Cabe resaltar que se usa como ventaja ese char que sobra para luego
	generar el buffer con el len en lugar de len + 1
*/

static int	calculate_total_length(char *token, char *value, char *key_to_find)
{
	int		len_of_key;
	int		len_of_token;
	int		len_of_key_value;
	int		total_len;

	len_of_token = ft_strlen(token);
	len_of_key_value = ft_strlen(value);
	len_of_key = ft_strlen(key_to_find);
	total_len = len_of_token + len_of_key_value - len_of_key;//no ay que hacer + 1 porque ya esta implicitamnete al no tener en cuenta el $
	return (total_len);
}

/*
	En esta función simplemente se toma el primera coincidencia con un '$'.
	Luego se verifica que sea valido, y si es asi, entonces se envia un ptr a
	esa posición. Si no lo es, se sigue buscando hasta que se encuentre
	o de un NULL como resultado.

	Ejemplo:

	"Hola $+ $. $USER" -> iteramos hasta $USER e ignoramos el resto
*/

static char	*find_dollar(char *token_val)
{
	char	*dollar;

	dollar = ft_strchr(token_val, '$');
	while (dollar)
	{
		if (*(dollar + 1)
			&& (ft_isalpha(*(dollar + 1)) || is_symbol(*(dollar + 1))))
			return (dollar);
		dollar = ft_strchr(dollar + 1, '$');
	}
	return (NULL);
}

/*
	Esta función es la que expande. Es inspirada en asig/check_asig/count_syntax
	Vamos a copiar todo lo que esta antes de la clave en el buffer que se
	generó con el tamaño exacto. Luego copiamos el valor de clave en lugar de
	la clave y luego copiamos lo que se encuentre después de la clave.

	Todo funciona utilizando aritmetica de ptrs en lugar de hacer
	mas llamadas al sistema de malloc, calloc o realloc con funciones como
	strdup, strjoin, etc...
	Me pareció más facil y directo de usar que utilizar memove, así
	como más entendible
*/

static void	expand(char *token_val, char **buffer, char *value, char *key)
{
	char	*dollar_ptr;
	int		key_len;
	int		before_var_to_replace;
	int		after_var_to_replace;

	dollar_ptr = find_dollar(token_val);
	key_len = ft_strlen(key);
	before_var_to_replace = dollar_ptr - token_val;
	after_var_to_replace = ft_strlen(dollar_ptr + 1 + key_len);
	ft_memcpy(*buffer, token_val, before_var_to_replace);
	ft_memcpy(*buffer + before_var_to_replace, value, ft_strlen(value));
	ft_memcpy(*buffer + before_var_to_replace + ft_strlen(value),
		dollar_ptr + 1 + key_len, after_var_to_replace);
}

/*
	Este es el último paso para expandir. Se calcula el len total y se usa
	para poder expandir. En lugar de hacer realloc se utiliza calloc y luego se
	libera.
*/

int	copy_value(t_shell *d, char **token_val, char *key_value, char *key_to_find)
{
	char	*new_buffer;
	int		total_len;

	total_len = calculate_total_length(*token_val, key_value, key_to_find);
	if (total_len <= 0) //when the key exist but the value is empty
		return (ERROR);
	new_buffer = ft_calloc(total_len + 1, sizeof(char));
	if (!new_buffer)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	new_buffer[total_len] = '\0';
	expand(*token_val, &new_buffer, key_value, key_to_find);
	free(*token_val);
	*token_val = new_buffer;
	return (SUCCESS);
}
