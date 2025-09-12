# Minishell
Minishell - Bash

TO DO LIST:
-bash + argumento deberia abrir ese archivo (solo el primero) y tratar de ejecutar los que haya dentro como input;
-Bash te abra linea para que completes el comando ver -> Modificar balance de ' " ( || && |
https://github.com/rogerdevworld/minishell/blob/main/src/bonus/loop_processing.c
https://github.com/rogerdevworld/minishell/blob/main/src/bonus/syntax/ft_lexer_2.c#L93

<h2>MIKEL:</h2>
<div>

- problemas de simplifly tokens ->  sds""''"$ls"''$mikel
genera doble NO_SPACE pero creo que no hay problema a malas se podria hacer una fucion que borre dobles NO_SPACES seguidos.

</div>

<h2>DAVID:</h2>
<div>
-arreglar is_cmd para que esto lo catalogue como cmd -> Token [3]: '/usr/bin/ls' (type: WORD)
-Cuando el usuario pone VAR="HOla" ->VAriable local. Tenerla lista para funncionamiento. <br>
-Busqueda del valor de la clave en la lista enlazada. Reorono dinamico. <br>
</div>
<br>
<br>

ERRORES
- (echo) hi -> deberia dar error syntax
- (echo hi && ((echo hi && (echo hi) && echo hi))) -> deberia dar error syntax

Tener en cuenta:
-expansion:echo "My home is $HOME" -> My home is /home/mikel (Comillas dobles) | echo 'My home is $HOME'-> My home is $HOME (comillas simples)

dia 1:

1. Arquitectura general
Vuestra shell va a seguir este flujo principal:

Prompt → Mostrar la línea y leer input.

Tokenización → Separar la entrada en tokens (palabras, operadores, etc.).

Expansión → Sustituir variables de entorno, tilde (~), y demás.

Construcción del AST → Crear un árbol sintáctico para modelar el comando y su estructura (pipes, redirecciones, etc.).

Ejecución recursiva → Navegar el AST y ejecutar cada nodo (manejo de fork, pipes, redirecciones).

Limpieza de memoria → Liberar todos los recursos de esta iteración.

2. Estructuras de datos clave
Definisteis un t_shell (o s_data) que encapsula todo el estado:

c
Copy
Edit
typedef struct s_data
{
    t_prompt tokens;  // Tokens del comando actual
    t_envp   envp;    // Variables de entorno
    t_parser ast;     // Árbol sintáctico del comando
} t_shell;
El objetivo es pasar t_shell como referencia para que todos los módulos trabajen sobre el mismo estado.

3. Main loop
Inicializa el estado (init).

Instala handlers de señales para el padre.

Bucle infinito:

Mostrar prompt → leer input.

Guardar en histórico.

Llamar a minishell() con el input leído.

Manejar errores.

4. minishell()
Secuencia interna:

c
Copy
Edit
if (!tokenizer(data, input))    return error(...);
if (!expansion(data->tokens, data->envp)) return error(...);
if (!ast(data, data->tokens))   return error(...);
if (!execute_recursive(data, data->ast))  return error(...);
clean_data(data);
Cada etapa devuelve false si falla y dispara limpieza y gestión de error.

5. Manejo de señales
Padre
Usa sigaction con un handler custom que:

No mata el proceso.

Limpia la línea y redibuja el prompt si está esperando input.

Marca un flag global para saber que ocurrió una señal.

Hijo
Restaura señales a SIG_DFL para que se comporten como en bash.

Si recibe SIGINT o SIGQUIT, muere como un comando normal.

El padre detecta con waitpid si el hijo terminó por señal y actúa.

Orden de desarrollo

1.Prompt + lectura de input.

2.Tokenizer básico.

3.Expansión de variables y comillas.

4.Parser para construir AST simple.

5.Ejecución de comandos

6.Señales y limpieza.

7.Refinar memoria y errores.


                ┌───────────────────────────┐
                │         MAIN LOOP         │
                │───────────────────────────│
                │  signals()  // PADRE      │
                │  init(&data)              │
                └─────────────┬─────────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │  prompt(&input)     │
                   │  (espera comando)   │
                   └─────────┬───────────┘
                 (SIGINT)    │   [EOF/exit]
                  PADRE      │   clean & exit
                  limpia     │
                  prompt     ▼
               ┌──────────────────────────────┐
               │ 1) TOKENIZER                 │
               │ - Divide en tokens           │
               │ - Identifica pipes, redirs   │
               │ - Maneja quotes              │
               └───────────────┬──────────────┘
                               ▼
               ┌──────────────────────────────┐
               │ 2) EXPANSION                 │
               │ - Sustituye $VAR, $?         │
               │ - Expande wildcards          │
               │ - Respeta quotes             │
               └───────────────┬──────────────┘
                               ▼
               ┌─────────────────────────────┐
               │ 3) AST BUILDER              │
               │ - Construye árbol sintáctico│
               │ - Agrupa comandos/args      │
               │ - Ordena pipes/redirs       │
               └──────────────┬──────────────┘
                              ▼
               ┌─────────────────────────────┐
               │ 4) EXECUTOR                 │
               │ - Recorre AST               │
               │ - fork() para comandos      │
               │ - Redirecciones             │
               │ - Pipes                     │
               └──────────────┬──────────────┘
                              │
                 ┌────────────┴─────────────┐
                 ▼                           ▼
      ┌─────────────────────┐     ┌─────────────────────┐
      │ PADRE               │     │ HIJO                │
      │──────────────────── │     │──────────────────── │
      │ - Espera con        │     │ - Restaura señales  │
      │   waitpid()         │     │   SIG_DFL           │
      │ - Maneja señales    │     │ - Ejecuta comando   │
      │   (Ctrl+C)          │     │ - Si señal → muere  │
      └─────────────────────┘     └─────────────────────┘
                 │                           │
                 │       (exit/señal)        │
                 └───────────┬───────────────┘
                             ▼
                ┌─────────────────────────────┐
                │ clean_data(data)            │
                │ Vuelve al MAIN LOOP         │
                └─────────────────────────────┘


DAY 02:
 La idea aqui es:

 HAcer un parseo inicial en el que se va tanto a contar cada cmd, pipe, quoute, parentesis, etc... y tambien asignar cada tipo a su respectiva structura t_token con toda su informacion.
 Luego, al finalizar el parseo se llama a:
 ft_validate_numbers-> Esta funcion verifica si el ocntador de cada tipo tiene concordancia con los demas y en general.
 ft_validate_token-> ESta funcion verifica que cada token tenga sentido en su contexto. POr ejemplo, si aunque hayan 2 parentesis, hay que revisar si hay conetido dentro de ellos o no. EL modo en que lo hacemos es recorriendo cada tipo desde el primer parentesis hasta el de cierre. Si hay mas tipos dentro, entonces hay concordancia y se puede proseguir. SI no, ERROR.
parseando(t_shell data, t_prompt *prompt)
{
	int i;
	char *start;
	int end;

	i = 0;
	while (str[i] != '\0')
	{
		if ( str[i] == '(')
		{
			ft_add_token(str[i]);
		}
		else if (str[i] != unitarios && != espacio)
		{
			start = str[i];
			while (str[i] != unitarios && != espacio)
			{
				end++;
			}
			ft_add_token(start + end);
		}
	}
	ft_add_token(t_tokens *tokens ,char *token)
	{
		static int i;
		tokens[i]->value = token;
		i++;
		if (falg)
			ft_parsing2(tokens);
	}

	ft_parsing2()
	{
		char **arr1 = {"(", ")", "\"", "\'", "||"...NULL}
		char ** arr2 = PARENTESIS, PARENTESIS CERRADO, COMILLA, ETC...NULL.};

		int i;
		while (tokens->value[i])
		{
			j = 0;
			if (ft_isalpha(tokens[i]->value) || ft_isdigit(tokens[i]->value))
			{
				tokens[i]->type = ft_check_if_word_or_cmd(tokens[i]->value);
			}
			else
			{
				while (arr1[j] != NULL)
				{
					if (tokens->value[i] == arr1[j])
					{
						if (token->value != $)
							tokens->exppand = 0;
						tokens->type = arr2[j];
					}
					j++;
				}
				i++;
			}
		}
	}

get_tokens
{
	while(input[i] != )
	{
		is_parentesis(token)
		is_comilla
		is_doble_comilla
		is_pipe
		is_semicolon if (cahr == ';')
		is_point
		is_wildcar
		is_and
		is_or
		is_heredoc
		is_redir
		is_word(token, &str[i] + i);
		is_finished
		i++
	}
}

hola |
is-word(char *word)
j = 0;
while (str[j] != ' ')
j++;

iswordcomadn(tokens, word, j );
is_word_or_command
{
	while(tokens[i])
	{
		is_command_is_word(tokens[i])
		is_quoted(tokens)
		is_expand(tokens)
		i++
	}
}
valid_tokens
{
	verify_sintaxis(tokens)
}



}
int ft_add_token(t_prompt *prompt, char *value)
{
	static int i = 0;
	prompt->tokens->array_tokens[i] = value;
	i++;
	return (0);
}

validate()
{
	if (data->prompt->n_parentesis % 2 == 0 )
		tot be
	else if (data->prompt->n_comd % 2 != 0)
	{
		clean(data);
	}
	if (data->prompt->cmd % 2 == 0 )
		tot be
	else if (data->prompt->n_parentesis % 2 != 0)
	{
		clean(data);
	}
	if (data->prompt->pipe % 2 == 0 )
		tot be
	else if (data->prompt->n_parentesis % 2 != 0)
	{
		clean(data);
	}
	if (data->prompt->n_parentesis % 2 == 0 )
		tot be
	else if (data->prompt->n_parentesis % 2 != 0)
	{
		clean(data);
	}
}

ERRORES SYNTACTICOS TOTAL:

# Paréntesis desbalanceados
( ls -l
ls -l )
echo ( ls )

# Comillas desbalanceadas
echo "hola
echo 'hola

# Pipes y redirecciones mal usados
| ls
ls |
ls | | grep txt
ls > | grep txt
ls >> | grep txt

# Operadores lógicos mal posicionados
&& ls
ls &&
ls || ||
ls && || echo hola

# Redirecciones sin operandos
ls >
ls <

# Combinaciones de operadores
ls && | grep txt
ls || > archivo
ls | && echo hola

# Heredoc mal formado
cat <<
cat << EOF

# Comillas y paréntesis mezclados
echo " ( hola )
echo ' ) hola ( '

# Wildcards y operadores
ls * &&
ls * |

ERRORES SYNTACTICOS POR RESOLVER:
echo 'hola
echo "hola
echo " ( hola )
ls && || echo hola
echo ' ) hola ( '
revisar error(ls | && echo hola)


TAREAS:

* PARENTESIS - QUOTES - NORMA \
* VARIABLE NUEVA - C_TOKENS


NOS SALVO LA VIDA:

# 1. Asegúrate de estar en la rama test y copia el hash del commit
git checkout test
git log --oneline   # copia el primer hash (el último commit)

# 2. Para cada rama que quieres que sea igual a test:
git checkout main
git reset --hard <hash_test>
git push -f origin main

git checkout ramaX
git reset --hard <hash_test>
git push -f origin ramaX

git checkout ramaY
git reset --hard <hash_test>
git push -f origin ramaY

# 3. Cuando todas estén ya iguales a test → elimina la rama test
git branch -D test
git push origin --delete test

OPCION PARA FLUJO DE TRABAJO:

1. Si hay token de expansion -> Se inteta expandir. Si no se puede, no importa, se ignora y no se innercambia por ningun \n.
2. enotonces se aplica una funcion de juntar (esto en caso de que haya algun NO SPACE token, si noy hay este token, no se junta nada).
3. Armamos el arbol
4. Antes de ejecutar si hay token de expansion -> Se intenta expandir (Ahora si de verdad, con \n y todo el rollo) y juntar obligatoriamente (si hay algo que juntar).
5. ejecutamos
