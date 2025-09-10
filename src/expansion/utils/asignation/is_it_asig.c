/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_it_asig.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:33:47 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/10 17:30:38 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	1. si hay un "="
	2. Que sea un grupo independiente
	3. Que no tenga detras un word o cmd o built-in o delante
	4. Si no es el primer token, solo puede tener && o || a su alrededor


	ASIGNACIÓN.

	Errores:
	1. 1VAR=.... ->
	2. VAR = ... -> Se ejecuta VAR como cmd y da error de ejecución. NO SE ASIGNA NADA.
	3. VAR= ...  -> A VAR se le asigna una cadena vacia y el resto se pone como word, de modo que el error es el WORD. SE ASIGNA CADENA VACIA
	4. VAR="HOLA -> Este error ya esta manejado.
	5. VAR=""HOLA"" ->  no error.
	6. CMD/BUILTIN VAR=Hola
	7. VAR=Hola CMD/BUILTIN

	Si el usuario hace una asignación de expansion:
	1. VAR="Hola"
	2. VAR=Hola
	3. VAR="Hola";
	4. VAR='Hola'
	5. VAR='Hola';
	6. VAR=(vacio) ->cadena vacia

	PROCEDIMIENTO:

	PRE-VERIFICACÓN:
	1. Verificación de =
	2. Verificación de sintaxis externa
	VERIFICACÓN:
	1. Sintaxis del token
	2. ASIGNACIÓN.
*/

/**
 * Sistema de análisis y ejecución de asignaciones en el shell.
 *
 * Este conjunto de funciones permite detectar, clasificar y ejecutar asignaciones
 * de variables en distintos contextos, como asignaciones locales o mediante el comando `export`.
 *
 * FLUJO 1: Reclasificación de tokens WORD → ASIGNATION
 * -----------------------------------------------
 * Objetivo: Identificar tokens tipo WORD que representan asignaciones válidas (ej. VAR=value)
 * antes de construir el AST, para facilitar su ejecución posterior.
 *
 * Funciones involucradas:
 * - `token_word_to_asignation`: recorre todos los tokens tipo WORD y evalúa si deben reclasificarse.
 * - `check_externs_syntax`: verifica el contexto sintáctico del token (tokens vecinos).
 * - `check_asignation_syntax`: valida la estructura interna del token (nombre válido, contiene `=`, sin espacios).
 *
 * Si ambas validaciones retornan TRUE, el token cambia su tipo a ASIGNATION.
 * Esto permite que el AST lo reconozca como nodo de asignación sin necesidad de revalidar en tiempo de ejecución.
 *
 *
 * FLUJO 2: Ejecución de asignaciones locales y por export
 * -------------------------------------------------------
 * Objetivo: Ejecutar asignaciones que ya han sido clasificadas como ASIGNATION.
 *
 * Funciones involucradas:
 * - `is_it_asig`: función principal que valida y ejecuta una asignación.
 *     - Llama a `check_externs_syntax` y `check_asignation_syntax` para confirmar validez.
 *     - Si es válida, llama a `asignation()` para realizar la operación.
 * - `asignation`: extrae clave y valor del token, limpia comillas y símbolos, y añade la variable al entorno.
 *
 * En el caso del comando `export`, se usa:
 * - `my_export`: recorre los tokens posteriores al comando `export`.
 *     - Ejecuta solo los tokens de tipo ASIGNATION.
 *     - Ignora otros tipos y se detiene ante operadores lógicos o estructuras de control.
 *     - Si no se ejecuta ninguna asignación, imprime las variables exportadas actuales.
 *
 * Este diseño modular permite separar claramente el análisis sintáctico, la clasificación semántica
 * y la ejecución, respetando el flujo clásico de un shell: parseo → AST → ejecución.
 */

int is_it_asig(t_shell *data, t_token *token, t_env *env, int type)
{
	int	result;

	result = FALSE;
	if (token->type == ASIGNATION)
	{
		if (ft_strchr(token->value, '='))
			check_externs_syntax(data, token, &result);
	}
	if (result == TRUE)
		result = asignation(data, token, type);
	return (result);
}
