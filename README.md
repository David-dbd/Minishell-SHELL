# Minishell
Minishell - Bash

🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
🏁                                     BONUS                                                       🏁
🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
             TOTAL TEST COUNT: 65  TESTS PASSED: 43  LEAKING: 0
                     STD_OUT: 20  STD_ERR: 14  EXIT_CODE: 7
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 41
                                     ✅ 154

🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
🏁                                    MANDATORY                                                    🏁
🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
             TOTAL TEST COUNT: 885  TESTS PASSED: 801  LEAKING: 0
                     STD_OUT: 52  STD_ERR: 43  EXIT_CODE: 35
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 130
                                     ✅ 2525

## POR HACER:

## MIKEL:
- cd cuando borras la carpeta
- que siempre se imprima el pwd de donde estes.

## DAVID:
- Important:
- Export
- Unset
- Variables
- Wildcars
- Expansiones

- get_arg_types funciona y sin leaks
- get_temp_asig funciona y sin leaks -> con export y con cualquier otro comando
- Expandir ~
- Expandir $$
- Expandir el Heredoc
- get_temp_asig debe funcionar correctamente y sin leaks -> con export y con cualquier otro comando
- bugs de tempasignations bien catalogadas
- bugs de export - unset
- bugs de expansiones

- Secondary:
- `echo $a && a=1`
- leaks en `export USER+=@@@`
- Expansion ha de soportar los wildcards *
- Norminette
- Comentarios

mikel@MSI ~/documents/cursus/rank03/minishell ❯ export PWD+="XXXX"
mikel@MSI ~/documents/cursus/rank03/minishellXXXX ❯ sd
minishell: sd: command not found
=================================================================
==1549155==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 10 byte(s) in 1 object(s) allocated from:
    #0 0x7fca0fb209c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x557ca4241787 in ft_calloc libft/ft_calloc.c:28
    #2 0x557ca422c320 in aux_mem_alloc src/expansion/utils/asignation/asignation.c:31
    #3 0x557ca422d097 in asignation src/expansion/utils/asignation/asignation.c:129
    #4 0x557ca423b6fc in asignation_type src/builtin/my_export.c:164
    #5 0x557ca423bb8b in my_export src/builtin/my_export.c:197
    #6 0x557ca4237aba in env_cmds src/executor/utils/which_builtin.c:34
    #7 0x557ca4237fe1 in which_builtin src/executor/utils/which_builtin.c:56
    #8 0x557ca4237613 in exec_builtin src/executor/exec/exec_builtin.c:70
    #9 0x557ca42356d7 in executor_recursive src/executor/executor.c:32
    #10 0x557ca420fe53 in main src/main/main.c:32
    #11 0x7fca0f7a51c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #12 0x7fca0f7a528a in __libc_start_main_impl ../csu/libc-start.c:360
    #13 0x557ca420fb84 in _start (/home/mikel/documents/cursus/rank03/minishell/minishell+0xcb84) (BuildId: ad722d2bb59b5508c96c8035214b2b410d7914aa)

SUMMARY: AddressSanitizer: 10 byte(s) leaked in 1 allocation(s).

## Errores:
- En is_double_quote -> no se pueden hacer dos expansiones seguidas cuando hay `\` antes del `$` funciona para expansiones
  individuales pero la flag seteada evita las demas expansiones
- Al expandir una variable que tenga () deberia volverse a tokenizar
- Deberia rehacerse todo la tokenizacion y expansion para que se consiga que cada token solo tenga una sola funcion argumento, operador(varios), Filename, heredoc comando o builtin, solo esas funciones. Nada de tokens de expansion deben marcarse antes con un bool pero no como un tipo y asi todo sera mas sencillo de gestionar.

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
