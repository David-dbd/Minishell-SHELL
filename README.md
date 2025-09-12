# Minishell
Minishell - Bash

<h2>POR HACER:<h2>
-Bash te abra linea para que completes el comando ver -> Modificar balance de ' " ( || && |
https://github.com/rogerdevworld/minishell/blob/main/src/bonus/loop_processing.c
https://github.com/rogerdevworld/minishell/blob/main/src/bonus/syntax/ft_lexer_2.c#L93

<h2>MIKEL:</h2>
<div>
- problemas de simplifly tokens ->  sds""''"$ls"''$mikel
genera doble NO_SPACE pero creo que no hay problema a malas se podria hacer una fucion que borre dobles NO_SPACES seguidos.
- is_double_quote manejar \ correctamente
- is_double_quote seguido de word -> NO_SPACE
- crear envp char **
- bash + argumento deberia abrir ese archivo (solo el primero) y tratar de ejecutar los que haya dentro como input;
- SHLVL actualizar

</div>

<h2>DAVID:</h2>
<div>
- Arreglar is_cmd para que esto lo catalogue como cmd -> Token [3]: '/usr/bin/ls' (type: WORD)
- Errores sintacticos
ERRORES
- (echo) hi -> deberia dar error syntax
- (echo hi && ((echo hi && (echo hi) && echo hi))) -> deberia dar error syntax
- Expansiones $?, $$, $!...
- actualizar el size de env cada vez que se setea con export
- my_assignation builin
- echo $a && a=1
- invalid read -> echo $A
- locales seteadas no dejan setear la misma variable a export -> no deberia ser asi
- no setea export A=2 ls D=5 correctamente como lo hace bash
- echo "hola $NOEXISTE" borra toda la string
- expansion ha de soportar los wildcars *
- refactorizar expansion para que incorpore send_tokens_for_expansion
- norminette
- comentarios
</div>


ARQUITECTURA GENERAL
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


OPCION PARA FLUJO DE TRABAJO:

1. Si hay token de expansion -> Se inteta expandir. Si no se puede, no importa, se ignora y no se intercambia por "".
2. enotonces se aplica una funcion de juntar (esto en caso de que haya algun NO SPACE token, si noy hay este token, no se junta nada).
3. Armamos el arbol
4. Antes de ejecutar si hay token de expansion -> Se intenta expandir (Ahora si de verdad, con "" y todo el rollo) y juntar obligatoriamente (si hay algo que juntar).
5. ejecutamos
