# Minishell
Minishell - Bash

🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
🏁                                     BONUS                                                       🏁
             TOTAL TEST COUNT: 71  TESTS PASSED: 54  LEAKING: 0
                     STD_OUT: 17  STD_ERR: 2  EXIT_CODE: ✓
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 19
                                     ✅ 194

🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
🏁                                    MANDATORY                                                    🏁
🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁
             TOTAL TEST COUNT: 882  TESTS PASSED: 815  LEAKING: 0
                     STD_OUT: 36  STD_ERR: 34  EXIT_CODE: 27
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 97
                                     ✅ 2549

## POR HACER:

## MIKEL:

## DAVID:
- Important:
- Variables
- Expandir el Heredoc
- bugs de export - unset
- Secondary:
- Norminette
- Comentarios

## Errores:
- En is_double_quote -> no se pueden hacer dos expansiones seguidas cuando hay `\` antes del `$` funciona para expansiones individuales pero la flag seteada evita las demas expansiones
- export no acepta todas las posibilidades de entrada por la menera en la que tokenizamos, tampoco export esta remplazando su variable de la misma manera porque a veces una variabel la entraga en varios argumentos.
- Al expandir una variable que tenga () deberia volverse a tokenizar, sucede que cuando tenemos una subshell en una variable y la expande no ha pasado por su proceso de tokenizacion y no es capaz de realizar el comando.
- Deberia rehacerse todo la tokenizacion y expansion para que se consiga que cada token solo tenga una sola funcion argumento, operador(varios), Filename, heredoc comando o builtin, solo esas funciones. Nada de tokens de expansion deben marcarse antes con un bool pero no como un tipo y asi todo sera mas sencillo de gestionar.
- tokenizamos los saltos de linea como ";" cuando bash depende del contexto lo hace de otra manera, en archivos si en linea de comando no
- El manejo de bool quotes para los tokens deberia mejorar mas que solo revisar si i - 1 es DOUBLE_QUOTE. 1quote 2quote 3hola ->hola:true
- funciones prohibidas que usamos:
localtime
difftime
mkdir
gethostname
getpid

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
