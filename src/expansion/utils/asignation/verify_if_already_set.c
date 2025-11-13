/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_if_already_set.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:36:26 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/05 22:38:31 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Verifica si una variable ya existe en el entorno y actualiza su valor.
 *
 * Este módulo gestiona la lógica de asignación de variables en el entorno
 * de minishell. Si la variable ya existe, se actualiza su valor según el tipo
 * de asignación (`LOCAL`, `ENV`, `EXP`, `PLUS_ASIGNATION`). Si no existe,
 * se indica que debe añadirse.
 *
 * Funciones principales:
 * - assign_or_replace_value: Asigna o reemplaza el valor de una variable
 	existente.
 * - handle_plus_assignation: Realiza la concatenación si se usa el operador
	`+=`.
 * - handle_existing_value: Coordina la actualización de valor según el tipo.
 * - update_variable_type: Ajusta el tipo de la variable si es necesario.
 * - verify_if_already_set: Punto de entrada que recorre el entorno y delega
 	la lógica.
 *
 * Uso típico: Llamado desde `asignation()` para decidir si se actualiza una
 	variable existente o se crea una nueva.
*/

static int	assign_or_ignore_or_replace_value(t_shell *data, t_var *var, char **value, int t)
{
	if (t == EXP && var->type == EXP)
	{// si solo es una palabra sin ""="" -> hola
		//printf("Ignore\n\n");
		return (IGNORE);
	}
	if (var->value && (t == LOCAL || t == ENV || t == TEMP_ASIGNATION))
	{// si existe su valor y es local o env, solo hay que cambiarlo
		//printf("if t == LOCAL\n");
		if (var->value)
			free (var->value);
		var->value = *value;
		update_envp(data);
	}
	if (!var->value && var->type == EXP && t != EXP)//Si no existe su valor, solo hay que agregarse
	{
		var->value = *value;
		update_envp(data);
	}
	return (0);
}

static void	handle_plus_assignation(t_shell *d, t_var *var, char **value, int t)
{
	char	*tmp;

	if (var->value && t == PLUS_ASIGNATION)
	{//Si existe su valor y es PLUS_ASIGNATION entonces hay que sumarla
		//printf("if t == PLUS_ASIG\n");
		//printf("%s=%s\n\n", var->key, *value);
		tmp = ft_strjoin(var->value, *value);
		if (!tmp)
		{
			free(var->key);
			free(*value);
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		}
		free (var->value);
		var->value = tmp;
		update_envp(d);
		//printf("var->value: %s\n\n", var->value);
	}
}

static int	handle_existing_value(t_shell *dat, t_var *var, char **value, int t)
{
	if (assign_or_ignore_or_replace_value(dat, var, value, t) == IGNORE)
		return (IGNORE);
	handle_plus_assignation(dat, var, value, t);
	return (0);
}

static void	update_variable_type(t_var *var, int t)
{
	if ((var->type == LOCAL && (t == ENV || t == EXP))
		|| (var->type == EXP
			&& (t == LOCAL || t == ENV || t == PLUS_ASIGNATION)))
		var->type = ENV;
	/*
	if ((var->type == LOCAL || var->type == EXP)
		&& (t == ENV || t == EXP)
		|| (var->type == EXP && t == LOCAL))
		var->type = ENV; //Si es var->type = EXP && t = EXP da igual porque ese
		//caso se maneja mas arriba. Lo puse aqui apra no tnere tantos if
	if (var->type == EXP && t == PLUS_ASIGNATION)
		var->type = ENV;*/
}

int	verify_if_already_set(t_shell *data, char *key, char **value, int t)
{
	t_var	*var;

	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			//printf("Found it-> %s=%s\n\n", key, *value);
			if (handle_existing_value(data, var, value, t) == IGNORE)
				return (IGNORE);
			update_variable_type(var, t);
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
}
