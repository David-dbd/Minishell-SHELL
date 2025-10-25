# Minishell
Minishell - Bash

🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
🏁                                    RESULT                                    🏁
🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
             TOTAL TEST COUNT: 881  TESTS PASSED: 726  LEAKING: 0
                     STD_OUT: 87  STD_ERR: 67  EXIT_CODE: 53
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 207
                                     ✅ 2436

## POR HACER:

## MIKEL:
- Important:
- Actualizar ultimo comando env
- env + argumento
- funcion que revierta los cambios de redirecciones porque en builtin
suceden en el padre y no en el hijo
- Test 137: ✅⚠️  ./missing.out
mini error = ( command not found)
bash error = ( No such file or directory)

- Secondary:
- Bash te abra línea para que completes el comando ver → Modificar balance de ' " ( || && |
  - https://github.com/rogerdevworld/minishell/blob/main/src/bonus/loop_processing.c
  - https://github.com/rogerdevworld/minishell/blob/main/src/bonus/syntax/ft_lexer_2.c#L93


## DAVID:
- Important:
- get_arg_types funciona y sin leaks
- get_temp_asig funciona y sin leaks -> con export y con cualquier otro comando
- bugs de tempasignations bien catalogadas
- Expandir ~
- Expandir el Heredoc
- revisar caso 'ex'port A=1 --> args_type

- Secondary:
- Hacer built-in CD
- `echo $a && a=1`
- invalid read `echo $SHLVL`
- invalid read `token: =1`
- invalid read y no asignacion correcta `export A=" Hola como estas = BIEN"`
- Explota en `export A=1 =`
- Memory leak `export A+=" como estas"`
- Expansion ha de soportar los wildcards *
- Norminette
- Comentarios
- (v=1) var=2 bash: syntax error near unexpected token `var=2'

## Errores:
- En is_double_quote -> no se pueden hacer dos expansiones seguidas cuando hay `\` antes del `$` funciona para expansiones
  individuales pero la flag seteada evita las demas expansiones

Entonces, ¿cuál es la regla?
Paréntesis crean un subshell, y todo lo que está fuera de él (después del redireccionamiento) se trata como comandos independientes.

Sin paréntesis, todo se analiza como parte del mismo comando, y los argumentos se agrupan hasta que aparece una redirección o un operador lógico (&&, ||, ;).

## ARQUITECTURA GENERAL

```
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
```

## OPCIÓN PARA FLUJO DE TRABAJO:

1. Si hay token de expansión → Se intenta expandir. Si no se puede, no importa, se ignora y no se intercambia por "".
2. Entonces se aplica una función de juntar (esto en caso de que haya algún NO SPACE token, si no hay este token, no se junta nada).
3. Armamos el árbol
4. Antes de ejecutar si hay token de expansión → Se intenta expandir (Ahora sí de verdad, con "" y todo el rollo) y juntar obligatoriamente (si hay algo que juntar).
5. Ejecutamos
