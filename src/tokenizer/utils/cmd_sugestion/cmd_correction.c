/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_correction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:00 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 00:50:36 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Heurística para detectar comandos mal escritos con una diferencia
 * mínima entre cadenas s1 y s2.
 *
 * Se usa para sugerir correcciones cuando el usuario comete errores
 * tipográficos al escribir comandos.
 *
 * Primero se compara la longitud de ambas cadenas. Solo se aceptan si
 * la diferencia es de ±1 carácter.
 *
 * Luego se recorre s2 y se avanza en s1 solo si los caracteres coinciden.
 * Esto permite tolerar errores leves.
 *
 * Al final se evalúa si las cadenas terminan con una diferencia mínima.
 * Si es así, se considera un match.
 *
 * Ejemplos:
 * - Inserción: "echop" vs "echo" -> (s1[j + 1] == '\0' && s2[i] == '\0')
 * - Eliminación: "eho" vs "echo" -> s1[j] == '\0' && s2[i + 1] == '\0'
 * - Reemplazo: "exho" vs "echo" -> j + 1 == len_1 && i == len_2) ||
 * 	(i + 1 == len_2 && j == len_1)
 * transposición
*/

static int	find_match(const char *s1, const char *s2)
{
	int	i;
	int	j;
	int	len_1;
	int	len_2;

	i = 0;
	j = 0;
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	if (!(len_1 - len_2 >= -1 && len_1 - len_2 <= 1))
		return (FALSE);
	while (s2[i] != '\0' && s1[j] != '\0')
	{
		if (s1[j] == s2[i])
			j++;
		i++;
	}
	if ((s1[j] == '\0' && s2[i + 1] == '\0')
		|| (s1[j + 1] == '\0' && s2[i] == '\0')
		|| (s1[j] == '\0' && s2[i] == '\0'))//si s1 esta contendio en s2 o si son iguales en len o si s1 se pasa por una letra
		return (SUCCESS);
	else if ((j + 1 == len_1 && i == len_2) || (i + 1 == len_2 && j == len_1)) //Seguir logica de abc vs axc
		return (SUCCESS);
	return (FALSE);
}

/*
	Esta función revisa el len del value. si su len == 1 se verifica que no sea
	un espacio, de modo que si el usuario hace " " el programa ofrezca una
	sugerencia que peude ser fastidiosa.
	También se revisa que sea diferente de "c" y "d" ya que "cd" es el cmd
	mas corto de todos, con lo cual solo quiero ofrecer una sugerencia si
	el usaurio hace c o d en lugar de ofrecerla siempre que introduzca
	caulquier otra letra, lo cual, de nuevo, es fastidioso.
*/

static int	is_valid_value(char *str)
{
	int	len;

	if (!str || !*str)
		return (FALSE);
	len = ft_strlen (str);
	if (len == 1 && str[0] == ' ')
		return (FALSE);
	else if (len == 1 && str[0] != 'c' && str[0] != 'd')
		return (FALSE);
	len = 0;
	while (str[len] != '\0')
	{
		if (is_symbol(str[len]) || str[len] == ' ')
			return (FALSE);
		len++;
	}
	return (TRUE);
}


int	process_token(t_shell *data, t_token *tokens, char **builtins, int i)
{
	int	result;
	int	j;

	j = 0;
	result = FALSE;
	while (j < data->builtins->len)
	{
		if (is_valid_value(tokens[i].value)
			&& find_match(tokens[i].value, builtins[j])) //si solo hay un caracter diferente
		{ //mayor que 1 porque el el built_in mas corto es cd y es de 1
			result = ask_confirmation(data, &tokens[i], builtins[j]);
			return (result);//Solo corrige el el primero que encuentra mal escrito en lugar de intnetar corregir todos ya que sería fastidioso para el usuario
		}
		j++;
	}
	return (IGNORE);
}

int	cmd_correction(t_shell *data, t_token *tokens, int n_tokens)
{
	char	**builtins;
	int		result;
	int		i;

	i = 0;
	builtins = data->builtins->builtins_selection;
	result = SUCCESS;
	while (i < n_tokens)
	{
		if (tokens[i].type == COMMAND)
		{
			result = process_token(data, tokens, builtins, i);
			if (result != IGNORE)
				break ;
		}
		i++;
	}
	if (!result)
		clean_prompt(&data->prompt);
	return (result);
}
