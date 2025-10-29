/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_alias_part_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:38 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/29 02:04:43 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../inc/minishell.h"

/*
	Función básica para revisar si el arg tiene algún otro simbolo que no
	sea el "-". Luego se llama a count_syntax que verifica que haya texto
	antes y después del simbolo "-".
*/

int	check_arg_syntax(char *arg, const char *built_in_err)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(arg);
	if (len < 1)
		return (FALSE);
	while (arg[i] != '\0')
	{
		if (!ft_isalnum(arg[i]))
		{
			if (strcmp(built_in_err, BUILTIN_ALIAS) == 0)
				ft_printf_fd(STDERR, ERR_BUILTIN_ALIAS, arg);
			else
				ft_printf_fd(STDERR, ERR_BUILTIN_UNALIAS, arg);
			return (FALSE);
		}
		i++;
	}
	return (SUCCESS);
}


static void	print_alias(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	printf("Total of assinged alias: %d:\n", i);
	while (cmd)
	{
		if (cmd->alias)
			printf("%s=%s\n", cmd->value, cmd->alias);
		cmd = cmd->next;
	}
}

int	process_pair_of_args(t_shell *d, t_cmd *cmd, char *cmd_arg, char *alias_arg)
{
	char	*cmd_value_copy;
	char	*alias_arg_copy;
	int		result;

	cmd_value_copy = ft_strdup(cmd_arg);
	alias_arg_copy = ft_strdup(alias_arg);
	if (!alias_arg_copy || !cmd_value_copy)
	{
		if (cmd_value_copy)
			free (cmd_value_copy);
		if (alias_arg_copy)
			free (alias_arg_copy);
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	}
	normalize_token_to_lower(cmd_value_copy);
	normalize_token_to_lower(alias_arg_copy);
	result = find_cmd(d, cmd, cmd_value_copy, alias_arg_copy);
	if (result == ERROR)
		return (ERROR);
	else if (!result)
		add_node_rule(d, cmd_value_copy, alias_arg_copy, INDIFERENT);
	return (SUCCESS);
}

/*
	my_alias funciona con pares de alias/built_in. Primero revisa que el alias
	sea conrrecto en cuanto a syntaxis y luego activa la bandera para poder
	empezar a buscar el siguiente argumento, que vendría siendo el "built_in".
	Si no encuentra el built_in en la lista entonces lo ignora y sigue con el
	siguiente. Si el número de argumnetos es impar no pasa nada ya que solo
	activa la flag de funcionaminto cuando encuentra un par. Todos los alias
	se normalizan a lowercase internamente para poder mantener siempre el mismo
	flijo y diseño del proyecto.

	Eg: alias hola cd
	args[i] -> cd;
	args[i - 1] -> hola

	Si no hay argumentos se imprimen todos los alias existentes asociados a
	sus built_ins.
*/

int	my_alias(t_shell *data, t_cmd *cmd, char **args)
{
	int	exit_flag;
	int	pair_flag;
	int	i;

	i = 0;
	exit_flag = 0;
	pair_flag = FALSE;
	if (!args || !args[0])
		print_alias(cmd);
	while (args[i])
	{
		if (pair_flag)
		{
			if (process_pair_of_args(data, cmd, args[i], args[i - 1]) == ERROR)
				exit_flag = EXIT_FAIL;
			pair_flag = !pair_flag;
			i++;
		}
		if (!check_arg_syntax(args[i], BUILTIN_ALIAS))
		{
			exit_flag = EXIT_FAIL;
			i += 2;
			continue ;
		}
		else
			pair_flag = !pair_flag;
		i++;
	}
	return (exit_flag);
}
