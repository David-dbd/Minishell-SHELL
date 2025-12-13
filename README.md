
# 🚀 Minishell Project: Technical Specification Overview

## Introduction

The Minishell project is a comprehensive and robust implementation of a custom command-line interpreter (shell), designed to meticulously emulate the core functionality and sophisticated behavioral intricacies of established Unix shells like **Bash** and **Zsh**.

Our goal was not merely to execute commands, but to engineer a pipeline capable of managing complex shell grammar, environment state, and process isolation with high precision.

Poject made by:<br></br>
**David Diaz: "David-dbd" on github - "davdiaz-" (42 login)**<br></br>
**Mikel Garrido: "lordmikkel" github - "migarrid" (42 loggin)**<br></br>

### Finished project. Adding extra features and new upgrades very soon!*
<br></br>
## Project Philosophy

The core design philosophy is centered on **Architectural Clarity** and **Contextual Integrity**.

This shell is built on the classic **Read-Eval-Print Loop (REPL)** structure, processing user input through distinct, heavily validated phases to ensure that execution logic is completely decoupled from parsing and interpretation logic.

#### Features:
* Command correction for builtins -> `echa: did you mean 'echo'?`
* Line for unbalanced prompts -> `echo hello &&`
* Script execution
* Local, Temporal and exported ASIGNATIONS and sync between them and the shell -> `var=1 ls - var=1 - export var=1`
* Wildcards -> `*hello - hello* - h*ell*o`
* Accepts commands on both uppercase and lowercase -> `ECHO/echo/EchO/eChO` all work
* Subshells
* Operands '&' and ';'
* UX for user experiecnce
* Reddirs and HEREDOC
* Expansions: `env variables - tilde - tilde plus - tilde minus`
* History
* Easy to read code and structure
  
<img width="524" height="244" alt="Captura de Pantalla 2025-11-25 a las 19 02 53" src="https://github.com/user-attachments/assets/cece2407-ef35-4cb0-a6d1-343ea142d34a" />


## 📇 Table of Contents

- [📚 Phase 1: Initialization & Tokenization](#-phase-1-initialization--tokenization)

- [🧩 Tokenization: Lexical Analysis and Token Semantics](#2-tokenization-lexical-analysis-and-token-semantics)

- [🛡️ Phase 3: Expansion Phase – Dynamic Substitution and Contextual Integrity](#3-expanded-phase-dynamic-substitution-and-contextual-integrity)

- [🧹 Phase 4: Token Simplification](#4-token-simplification)

- [✨ Phase 5: Wildcard Expansion](#5-wildcard-expansion)

- [🌳 Phase 6: Abstract Syntax Tree (AST) Construction](#6-abstract-syntax-tree-ast-construction)

- [🚀 Phase 7: AST Execution Phase](#7-ast-execution-phase)

- [🎯 Phase 8: Assignment Engine – Scope, Persistence, and State Management](#8-assignment-engine--scope-persistence-and-state-management)

- [🛠️ Phase 9: Built-in Command Execution](#9-built-in-command-execution)

- [🧬 Phase 10: Minishell Data Model – Core Structures and Types](#10-minishell-data-model--core-structures-and-types)

- [✨ Phase 11: Extra Features – UX and Robustness](#11-extra-features--ux-and-robustness)

---
# 📚 Phase 1: Initialization & Tokenization

## 1\. ⚙️ Initialization, Environment Setup, and Signal Handling

The Initialization phase establishes the essential foundation for the shell's operation, ensuring proper resource management, environmental context, and responsiveness to system events.

### 1.1. The Global State Structure (`t_shell`)

The `main` function starts by declaring and initializing the **`t_shell data`** structure. This structure serves as the single source of truth for the entire program, holding links and data required across all execution stages:

  * **Execution Context:** Stores the current state of the process, including file descriptors, exit codes (`data.exit_code`), and the execution status.
  * **Prompt/Token Link:** Contains the `t_prompt` structure, which manages the user input and the dynamic array of tokens for the current cycle.
  * **Environment:** Holds the parsed environment variables (copied from `envp`) in a manageable structure, enabling internal commands like `export` and `unset` to modify the shell's environment.
  * **AST Root:** A pointer to the root of the Abstract Syntax Tree (`data.ast_root`), which is populated after tokenization and parsing.

### 1.2. The Main Execution Loop (REPL)

The core shell logic is managed by an infinite **Read-Eval-Print Loop (REPL)** implemented in `main.c`:

```c
int main(int argc, char **argv, char **envp)
{
	t_shell	data;

	init_minishell(&data, argc, argv, envp);
	while (receive_input(&data, &data.prompt) != NULL)
	{
		if (!tokenizer(&data, &data.prompt, data.prompt.input))
			continue ;
		ast_builder(&data, data.prompt.tokens, data.prompt.n_tokens);
		executor_recursive(&data, data.ast_root, &data.exec, FATHER);
		clean_cycle(&data.exec, &data.prompt, &data.ast_root);
	}
	exit_succes(&data, MSG_GOODBYE, data.exit_code);
	return (data.exit_code);
}
```

#### **Key Design Decisions:**

  * **Input Acquisition:** Instead of standard `readline`, the custom library **`iscoline`** is employed for reading user input. This decision was made to avoid known memory leaks and potential malfunctions associated with certain `readline` implementations, ensuring robustness.
  * **Error Handling:** If the **`tokenizer`** function detects a syntax error, it returns `SYNTAX_ERROR` (or `FAILURE`), causing the `if (!tokenizer(...))` condition to fail. The `continue` statement immediately skips the AST construction, execution, and cleanup for the current cycle, returning control to the start of the `while` loop to await new input.
  * **Cycle Cleanup:** The `clean_cycle` function is crucial. It frees all memory associated with the processed command (tokens, AST nodes, execution variables), guaranteeing that the shell starts the next input cycle from a clean memory state, thus preventing incremental memory leaks.

### 1.3. Robust Signal Management

Proper signal handling is paramount for a shell to behave reliably. This is managed using the `signal()` function setup during initialization and relies on a **global array** (`global_arr[2]`) to track state:

  * **`global\_arr[0]` (Signal Number):** Stores the last received signal (e.g., `SIGINT`, `SIGQUIT`).
  * **`global\_arr[1]` (Mode):** Indicates the current execution context (e.g., `HEREDOC`, `INTERACTIVE`).

**Impact:** By centralizing signal state management, the shell can execute different signal handlers based on the current mode (e.g., ignoring $\text{Ctrl}+\text{C}$ in the parent shell while executing a child process, or handling $\text{Ctrl}+\text{C}$ to break out of a `heredoc` input prompt).

-----

# 2\. 🧩 Tokenization: Lexical Analysis and Token Semantics

The Tokenization phase transforms the raw input string into a structured, executable list of tokens.

### 2.1. The Token Data Structure (`struct s_token`)

Every logical unit recognized by the tokenizer is stored in a `struct s_token`. The design includes specific fields to maintain the integrity and traceability of data throughout the execution pipeline.

| Member | Type | Purpose and Impact |
| :--- | :--- | :--- |
| **`id`** | `int` | **Dynamic Index:** Serves as the current array index. This value is **updated** every time the token array is modified (e.g., token deletion, reorganization, or simplification) to ensure efficient array traversal. |
| **`hash`** | `int` | **Permanent Identifier:** A fixed, unchangeable value initially set to `id`. Its primary purpose is to act as a permanent **key** to link the token to its corresponding node in the Abstract Syntax Tree (AST) and to reference persistent data structures (like `heredoc` files), preventing connection loss during token array reconfigurations. |
| **`type`** | `t_type` | **Semantic Category:** Identifies the token's role (e.g., `COMMAND`, `BUILTIN`, `EXPANSION`, `PIPE`). This is the key information used by later phases (Expansion, AST Builder) to target specific tokens for processing. |
| **`value`** | `char *` | **Raw Content:** The string data extracted from the input using `ft_substr` (e.g., `"ls"`, `"USER"`, `""`). |
| **`single_quoted`** | `bool` | Indicates if the token was surrounded by single quotes (`' '`). Crucial for the **Expansion Phase**, as content inside single quotes is protected from variable expansion. |
| **`double_quoted`** | `bool` | Indicates if the token was surrounded by double quotes (`" "`). Crucial for the **Expansion Phase**, as variable expansion **is** permitted within double quotes, but wildcard expansion is not. |
| **`expand`** | `bool` | **Expansibility Flag:** A general flag indicating whether the token is eligible for environment variable expansion. Tokens like `WORD` and `EXPANSION` may have this set to `TRUE`. |
| `wildcard_info` | `t_wild *` | Pointer to metadata required for the Wildcard Expansion phase. |

### 2.2. Dynamic Token Array Management

Tokens are stored in a dynamically allocated array (`prompt->tokens`). This provides cache efficiency while accommodating command lines of arbitrary length.

#### **Dynamic Capacity Logic (`check_buffer.c`):**

The `check_buffer` function ensures the array never overflows by dynamically resizing the underlying memory structure before a new token is added:

```c
void check_buffer(t_shell *d, t_prompt *p)
{
	size_t	new_capacity;
	t_token *new_tokens;

	if (p->n_tokens >= p->n_alloc_tokens)
	{
		new_capacity = p->n_alloc_tokens * 2;
        // ... safety check for INT_MAX ...
		new_tokens = ft_realloc(p->tokens,
				p->n_alloc_tokens * sizeof(t_token),
				new_capacity * sizeof(t_token));
        // ... error handling ...
		p->tokens = new_tokens;
		p->n_alloc_tokens = new_capacity;
	}
}
```

  * **Strategy:** The array capacity (`p->n_alloc_tokens`) is **doubled** when the token count (`p->n_tokens`) meets the current limit.
  * **Impact:** This design provides an **amortized $O(1)$** performance for adding tokens, making the tokenizer highly efficient even for extremely long command lines.

### 2.3. Token Creation and Sanitization (`get_tokens`)

The `get_tokens` function iterates through the input string, applying a strict order of precedence to identify tokens:

```c
void get_tokens(t_shell *data, t_prompt *prompt, char *input)
{
	int i = 0;
	while (input[i] != '\0')
	{
		is_not_token(input, &i); 		// Skip whitespace, comments
		is_and(...); is_or(...); 		// Logical operators (high precedence)
		is_pipe(...); is_parenten(...); 
        // ... other special/delimiter tokens ...
		is_single_quote(...); 			// Quote handling (takes precedence over word)
		is_double_quote(...);
		is_wildcar(...);
		is_dolar(...); 					// Expansion (takes precedence over generic word)
		is_word(...); 					// Catch-all generic word
        // ...
	}
    // ... post-processing
}
```

#### **Integrated Cleaning and Sanitization:**

Crucially, token creation functions do not just extract substrings; they actively clean and sanitize the content for later processing:

  * **`cleanner_word` / `cleanner_exp`:** These functions (found, for example, in `is_double_quote.c` and `is_dolar.c`) eliminate characters that are necessary for token boundaries but undesirable in the final token value.
      * **Example:** For variable expansion syntax, `cleanner_exp` removes superfluous characters like the braces (`{` and `}`) from $\$\text{\{VAR\}}$, leaving only the variable name `VAR` as the token value.
  * **`is_ignore_token`:** This utility is used to handle sequences that must be tokenized but which are meant to be ignored or treated specially, such as positional parameters (`$1`, `$2`), which are typically ignored by shells in an interactive context.
  * **Command Eager Classification:** As detailed in the previous phase, `is_cmd` is called immediately on new `WORD` tokens to classify them as `COMMAND` (external executable) or `BUILTIN` (internal function), streamlining the subsequent execution process.

### 2.4. Semantics and Syntax Validation

Immediately following token generation, the `check_if_valid_tokens` function performs a comprehensive sweep of the token array to enforce Bash syntax rules, ensuring the command structure is semantically viable.

```c
int check_if_valid_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
    // ... loop through all tokens ...
    // ... validation checks ...
	if (!check_parent_balance(data, prompt, tokens))
		return (SYNTAX_ERROR);
	return (SUCCESS);
}
```

#### **Core Validation Areas:**

  * **Delimiter Placement:** Functions like `check_pipe`, `check_or_and`, and `check_redir_*` ensure that operators (e.g., `|`, `&&`, `<`) are correctly flanked by valid tokens (commands, words, filenames, parentheses). For example, `check_pipe` confirms tokens exist both **before and after** the `PIPE`.
  * **Parentheses Balance:** `check_open_parent` and `check_close_parent` ensure not only that parentheses are balanced but also that their placement is logically correct (e.g., preventing `cmd(cmd)` or `( | )`). The final `check_parent_balance` verifies the overall count.
  * **Quote Closure:** Functions like `check_double_balance` and `check_single_balance` verify that all quotes in the input are properly closed.

#### **Error and Cleanup Protocol:**

When a syntax error is detected (e.g., in `check_pipe`):

1.  A dedicated error function, such as `syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value)`, is called.
2.  This error function prints the specific error message to the user, sets the correct exit code, and then triggers a cleanup sequence.
3.  The validation loop immediately returns **`SYNTAX_ERROR`**.

This robust process guarantees that, upon failure, all memory related to the faulty command line is freed, and the program flow returns directly to the **main loop's `continue`** statement, ready for the next user input without attempting to execute the invalid command.
-----

### 2.5. 🚨 Semantics and Syntax Validation: Error and Cleanup Protocol

The `check_if_valid_tokens` function runs immediately after token generation to enforce fundamental Bash syntax rules. This mechanism ensures the shell **fails fast** on invalid input, preventing resource waste in later, more complex stages.

#### **Core Validation and Error Flow:**

1.  **Validation Check:** A function (e.g., `check_pipe`, `check_open_parent`) performs a semantic rule check on the current token.
2.  **Error Trigger:** If a violation is detected (e.g., `|` without a preceding command), the function immediately calls the central error handler:
    ```c
    syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
    ```
3.  **Error Handling (`error.c` Logic):**
      * **Reporting:** The `syntax_error` function uses variadic arguments (`va_list`) to construct and print a detailed error message to `STDERR` (standard error).
      * **Cleanup:** It immediately calls **`clean_prompt(&data->prompt)`** to free all tokens and the input buffer memory associated with the faulty command line. This is a crucial step to avoid incremental memory leaks.
      * **State Update:** It sets the shell's global **`data->exit_code`** to the provided error code (e.g., `2` for `EXIT_USE`), allowing the shell to reflect the error status.
4.  **Loop Termination:** The validation check returns **`SYNTAX_ERROR`** (which maps to `FAILURE`), causing the main loop in `main.c` to trigger the `continue` statement, returning control to the start of the REPL to await the next user input.

This explicit protocol guarantees that all resources related to an invalid command are released before the next cycle begins.

-----

# 3\. 🛡️ Expansion Phase: Dynamic Substitution and Contextual Integrity

The Expansion Phase is the engine of variable substitution, where raw token values (`\$VAR`, `~`) are transformed into their environment-defined counterparts. This stage is engineered with a **two-phase system** to maintain contextual integrity, preventing the critical state-dependency flaws common in single-pass shell implementations.

## 3.1. Engineering a Robust, Two-Phase System

The Minishell expansion is divided into **two distinct phases** to manage dependency and execution order, ensuring that variable substitution occurs only when the context is stable.

| Phase | Core Logic | Dependencies and Purpose |
| :--- | :--- | :--- |
| **INITIAL\_PHASE** (`initial\_expansion\_process`) | Expands all tokens **that can be safely expanded** (i.e., those whose value is not being changed by a same-line assignment). | **Pre-Processing & Simplification:** Allows immediate token simplification and prepares the token list before AST construction begins. |
| **FINAL\_PHASE** (`final\_expansion\_process`) | Expands all remaining tokens, particularly those that **depend on a runtime context** (i.e., those blocked by `dont\_expand\_this`). | **Contextual Execution:** Ensures variables modified in the current command line receive their correct, final value before execution. |

-----

## 3.2. 💡 The `dont_expand_this` Solution: Preserving State Integrity

⚠️ _The Minishell Flaw:_ State-Dependency IssueBefore implementing the two-phase system, Minishell suffered from a critical flaw common to many simple shell designs: Premature Expansion due to State Dependency.The Problem of Immediate Expansion In the standard shell execution flow, tokens are expanded immediately after being read and categorized. This creates an incorrect order of operations for commands involving same-line variable assignments:

`$$\text{VAR}=\text{NEW\_VALUE}; \text{echo} \ \$ \text{VAR}$$`

Read/Expand: The shell would tokenize and expand $VAR first. It would substitute the old value of VAR from the environment.Execute: Only later, during the execution stage, would the assignment VAR=NEW_VALUE occur.Result: The echo command would execute with the stale, old value, violating the user's intent to use the NEW_VALUE set in the same command line. Your solution, dont_expand_this, was engineered specifically to detect this pending state change and defer the expansion, thereby ensuring contextual integrity between assignment and variable usage.

### 3.2.1. The Minishell Flaw & Dependency Issue

Consider this input: `USER=David; echo hello $USER`

  * A simple, single-pass expansion would see `echo hello $USER`. If `USER` was previously set to `Alex` in the environment, the shell would expand `$USER` to `Alex` **immediately** during the tokenization stage.
  * The assignment `USER=David` occurs *later* during execution.
  * **Resulting Flaw:** The command executed would be `echo hello Alex`, even though the user intended the command to be `echo hello David`. The expansion failed to recognize the upcoming change in state.

### 3.2.2. The Solution Logic (`dont_expand_this`):

The function `dont_expand_this` (within `initial_expansion_process`) acts as a crucial sentinel against this premature expansion:

1.  **Assignment Recognition:** The system first analyzes all tokens to correctly identify valid **Assignment Tokens** (e.g., `VAR=Hello`) by checking their syntax and semantics, distinguishing them from ordinary `WORD` tokens.
2.  **Value Comparison:** For every Assignment token (`KEY=VALUE`), the function performs a lookup using:
    ```c
    char *get_var_value(t_var *vars, const char *key);
    ```
      * **Case 1: No Change:** If the value being assigned (`VALUE` = "David") is **identical** to the current value stored in the environment list (`vars` = "David"), the expansion is safe. Tokens attempting to expand `$KEY` can proceed in the INITIAL\_PHASE.
      * **Case 2: Change Detected (Block):** If the assigned value ("David") **differs** from the current environment value ("Alex"), a change is pending.
3.  **Blocking Expansion:** To prevent the expansion flaw, the system iterates over the token array and finds all pending `EXPANSION` tokens matching the key (`$USER`). It sets a dedicated boolean flag:
    ```c
    bool expand = FALSE;
    ```
    This signal tells the main expansion routines to **ignore** these tokens in the INITIAL\_PHASE, effectively blocking them until the FINAL\_PHASE, where the state change will have been executed.

**Impact:** By deferring expansions that rely on a pending state change, this system ensures **contextual integrity**. The `EXPANSION` tokens are held in their raw `\$VAR` form until the execution phase is ready to handle the new state.

-----

## 3.3. Expansion Mechanics and Substitution Pipeline

The core substitution logic (`copy_key`, `find_key_in_list`, `copy_value`) is called repeatedly by the main `expansion` function for every variable found in an eligible token.

### 3.3.1. Key Extraction and Lookup

  * **Extraction (`copy_key`):** Scans the token's value string to find the first valid `$` or `~`, enforcing shell rules (alphanumeric keys, special tilde forms). The key name is extracted into a buffer (`key\_to\_find`).
  * **Value Retrieval (`find_key_in_list`):** Searches the environment linked list (`d->env.vars`) for the matching key. It also handles special symbols (`$??`, `$$`) during the FINAL\_PHASE.

### 3.3.2. Efficient Substitution (`copy_value.c`)

Once the environment value is found, the **`copy_value`** function executes the in-place replacement:

1.  **Length Calculation:** `calculate_total_length` determines the exact size of the resultant string.
2.  **Expansion Logic (`expand`):** A new buffer is allocated. Using efficient **pointer arithmetic and `ft_memcpy`**, the function copies the string segments:
      * String **before** the `$` $\rightarrow$ **Value** from environment $\rightarrow$ String **after** the key.
      * This is a high-performance approach, avoiding multiple expensive allocation calls (like `strjoin`) typically required for string manipulation.
3.  **Token Update:** The original `token->value` is freed, and the token pointer is updated to the new, expanded buffer.

### 3.3.3. Handling Unfound Variables (`expand_empty_str`)

If a variable is not found in the environment, `expand_empty_str` ensures the token list remains syntactically correct:

  * **Unquoted & Standalone (`$UNSET`):** The token is **eliminated** from the array.
  * **Quoted & Standalone (`"$UNSET"`):** The token is replaced by a **single space**, preventing adjacent tokens from merging.
  * **Embedded (`text$UNSET`):** The `$UNSET` substring is removed, but the remaining `text` is preserved.

-----

## 3.4. FINAL\_PHASE: Contextual Transformations and Synchronization

The `final_expansion_process` is executed during the command setup phase and manages array instability introduced by expansion.

1.  **Node-Specific Expansion:** Expansion is deliberately limited to the tokens relevant to the current **`t_node`** (command arguments). This scoping prevents unintended interference between different commands in a complex pipe or list.
2.  **Simplification:** **`simplify_tokens`** is called to merge adjacent `WORD` tokens and clean up residual `NO_SPACE` tokens left by the tokenizer.
3.  **Synchronization (Hashing):** Because simplification, word splitting, and wildcard expansion physically rearrange the token array, the link to the AST is lost. **`reconect_nodes_tokens`** is called after every array-modifying step, using the tokens' **`hash`** (the permanent key) to re-establish the connection between the AST node and its corresponding token index (`id`).
4.  **Word Splitting (`split_expansion_result`):** If an unquoted expansion resulted in a value with whitespace, this function splits the expanded token into multiple new `WORD` tokens.
5.  **Wildcard Expansion (`expand_wildcards`):** The final step performs pattern matching.
6.  **Argument Finalization:** The resulting tokens are collected and assembled into the `char **args` array for the executor.

-----

## 3.5. 💾 Engineering for State Preservation: The Role of before_tokens_type
The prompt->before_tokens_type array is a temporary, crucial data structure used to save the original token types immediately before the expansion and simplification process begins.

Necessity for Contextual Splitting
After an EXPANSION token is substituted, its resulting type is often changed to WORD. The challenge lies in determining if this new WORD token should be subject to Word Splitting (splitting the token by internal whitespace) or if it was originally protected by quotes.

The Problem: In Bash, word splitting only applies to an expanded token if the original token was unquoted (e.g., $VAR). If the original token was double-quoted ("$VAR"), its expanded content must remain a single token, even if it contains spaces.

The Solution: The split_expansion_result function cannot rely on the token's current type or quoting flags alone. By referencing the saved before_tokens_type array, the system can definitively look back and confirm:

What type was this token originally? (It must have been an EXPANSION).

What was the surrounding context? (Was it an unquoted EXPANSION?).

This look-back mechanism allows Minishell to correctly apply or suppress word splitting, ensuring strict adherence to the complex rules of shell expansion integrity.

---

## 3.6. 📐 Contextual Simplification Rules: `prepare_simplify` and `DONT_ELIMINATE`

The **`prepare_simplify`** function is a crucial pre-processing step within the `final_expansion_process`. Its sole purpose is to audit the token array for specific boundary conditions created by expansion results (such as an `$VAR` being eliminated) and override the default joining behavior of the upcoming `simplify_tokens` function.

### The Problem: Unintended Token Concatenation

The `simplify_tokens` function joins tokens that are separated by a **`NO_SPACE`** token. However, if an adjacent `EXPANSION` token results in an empty string and is eliminated, the `NO_SPACE` might shift and become adjacent to two tokens that should *not* be joined (e.g., a command and its argument), leading to an incorrect command structure.

### The Solution: Rule-Based Boundary Protection

`prepare_simplify` iterates through the tokens, focusing on indices where the **original token type** (`before\_tokens\_type`) was `EXPANSION` and the current adjacent token is a `NO_SPACE`. It then applies a strict set of contextual rules (`check_cases`) to stabilize the array:

#### **Key Stabilization Rules:**

1.  **Protecting Commands:** If a `NO_SPACE` is located between a **Command/Built-in** token and a subsequent token that is *not* another `NO_SPACE`, the central `NO_SPACE` token is converted to **`DONT_ELIMINATE`**.
    * **Impact:** This prevents the command token from being accidentally concatenated with the following word, preserving the command boundary.

2.  **Protecting Literal Words:** If a `NO_SPACE` is positioned between two **`WORD`** tokens, it is converted to **`DONT_ELIMINATE`**.
    * **Impact:** This maintains intended separation between the two distinct words, which is necessary if the `NO_SPACE` was part of a complex expression that resolved into two separate words that should not be fused.

3.  **Eliminating Redundancy:** If a `NO_SPACE` token is found immediately adjacent to another `NO_SPACE` token, the token at the current index is **eliminated** immediately.
    * **Impact:** This tidies the array, removing unnecessary duplication and ensuring clean boundaries. The array is immediately reconnected (`reconect_nodes_tokens`) after elimination to restore the AST link.

By strategically transforming the `NO_SPACE` token type to **`DONT_ELIMINATE`**, Minishell forces `simplify_tokens` to skip joining that particular segment, thus guaranteeing that the structural integrity of commands and arguments is maintained following dynamic expansion.

---

# 4. 🧹 Token Simplification

The Simplification Phase is the clean-up and consolidation stage of the pipeline. Its primary objective is to finalize word formation by **concatenating adjacent, related tokens** and **removing non-semantic markers** (like quotes and `NO_SPACE` tokens) to produce a dense, accurate sequence of final command arguments ready for the Abstract Syntax Tree (AST) builder.

## 4.1. Core Logic: Identifying and Joining Ranges

The central function, `simplify_tokens`, iterates through the token array, searching for **`NO_SPACE`** tokens which act as flags indicating that adjacent content should be fused.

### 4.1.1. Range Detection (`get_no_space_range`)

Simplification operates on a *range* defined by a `NO_SPACE` marker:

1.  **Search:** `get_no_space_range` finds the next `NO_SPACE` token in the stream.
2.  **Boundary Definition:** It calls `find_range_start` and `find_range_end` (`adjust_range_tokens.c`) to determine the exact index range $[R_{\text{start}}, R_{\text{end}}]$ of the tokens that need to be merged.
    * **Complex Rule Set:** The range functions incorporate complex heuristics to correctly identify tokens that belong together, including:
        * **Quoted Segments:** Tokens enclosed by opening and closing quotes (`SINGLE\_QUOTE`, `DOUBLE\_QUOTE`) are always included in the range to ensure the entire quoted segment is treated as one unit.
        * **Consecutive Quotes:** They handle complex scenarios where multiple tokens are involved in a single logical string (e.g., `cmd"word1""word2"`), ensuring the full sequence is captured for joining.

### 4.1.2. Token Concatenation (`join_tokens`)

Once a valid range is found:

1.  **Feasibility Check:** `is_possible_simplify` ensures the range is viable (contains at least one `NO_SPACE` marker and does not contain unprocessed `EXPANSION` tokens).
2.  **String Fusion:** `join_tokens` iterates through the tokens in the identified range. It allocates a new, final string (`result`) and uses **`ft_strjoin`** repeatedly to concatenate the `value` strings of all tokens within the range.
3.  **Efficiency:** The concatenation process targets only tokens that are relevant (`is_needed_to_simplify`), ensuring efficiency by skipping purely structural tokens (like `NO_SPACE` itself).

## 4.2. Managing Array Instability (`reorganize_tokens`)

The most complex task in this phase is managing the array after a fusion operation, as replacing multiple tokens with a single token creates a **gap** in the array.

### 4.2.1. Reorganization Protocol

The `reorganize_tokens` function manages the transition from the old, fragmented tokens to the new, simplified token:

1.  **Preserve Hash:** The `hash` of the token at the start of the range (`tokens[range[0]]`) is saved. This is critical because the new simplified token inherits the original hash, preserving the **permanent link** to the AST node (established during the Expansion Phase).
2.  **Resource Cleanup:** `free_tokens_in_range` is called to free the memory (`value` strings) of all tokens in the old, redundant range.
3.  **Token Replacement:** The newly created, concatenated string (`res`) is assigned to `tokens[range[0]].value`, and its type is set to `WORD`.
4.  **Array Collapse:** `ft_memmove` is used to shift all tokens following the range ($R_{\text{end}} + 1$) forward to fill the created gap.
    * **Impact:** This is a high-performance operation that physically collapses the array, maintaining contiguous memory.
5.  **State Update:** The total token count (`p->n_tokens`) is decreased by the number of tokens removed, and `void_tokens_at_the_end` initializes the memory at the end of the array to zero.

### 4.2.2. Dedicated Edge Case Cleanup

Before and during simplification, specific helper functions enforce boundary rules related to delimiters and line endings (`reorganize_tokens.c`):

* **`no_space_at_delimiter`:** If a `NO_SPACE` is found immediately preceding a **delimiter** (e.g., `|`, `&&`), the `NO_SPACE` is immediately eliminated. This prevents logic errors that could arise if a simplified range included a delimiter.
* **`no_space_at_end`:** If a `NO_SPACE` token is the very last token in the array (a common result of expansion failures), it is eliminated, tidying the final array state.

## 4.3. Final Cleanup: Quote Removal

The final step in `simplify_tokens` is structural cleanup:

* **`remove_quotes_tokens`:** This function iterates through the entire array and physically removes all remaining **quote tokens** (`SINGLE\_QUOTE`, `DOUBLE\_QUOTE`).
* **Rationale:** The quotes have served their purpose—they protected content during tokenization and expansion, and defined the boundaries for simplification. Since their content is now fully fused, the quote markers themselves are non-semantic and are removed using a two-pointer technique (read/write indices) to complete the array collapse.
* **Final ID Update:** The array size is updated, and **`adjust_id`** is called one last time to ensure the dynamic `id` of every token accurately reflects its final index in the array, making it ready for the AST builder.

```c
void	simplify_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	range[2];

	i = 0;
	while (i < prompt->n_tokens && tokens[i].type)
	{
		if (get_no_space_range(tokens, range, i, prompt->n_tokens))
		{
			if (is_possible_simplify(tokens, range))
			{
				if (no_space_at_end(data, prompt, tokens)
					|| no_space_at_delimiter(data, prompt, tokens))
					return ;
				join_tokens(data, prompt, tokens, range);
				i = range[0] + 1;
				continue ;
			}
		}
		i++;
	}
	remove_quotes_tokens(prompt, tokens);
	adjust_id(tokens, prompt->n_tokens);
}
```

---

# 4.4 🔄 Token Transformation Logic

The Transformation Phase is a complex, multi-pass refinement system that runs after simplification to finalize the semantic role of every token. The difficulty lies in the ambiguity of the command line, where a sequence like `VAR=value` can be either an **argument** (if preceded by a command) or an **assignment** (if at the start of the line).

## 4.5. 👑 The Hardest Part: Accurate Assignment Detection

The core engineering challenge in this phase is the precise classification of tokens that adhere to the assignment syntax (`KEY=VALUE`):

$$
\text{Initial Ambiguity: Is } \text{VAR}=\text{value} \text{ an argument or an assignment?}
$$

The logic employs a **two-tiered validation** system (`transform_word_to_asignation` and `check_externs_syntax`) to ensure a token only retains the `ASIGNATION` type if both its *internal syntax* and *external context* are valid.

### 4.5.1. Internal Syntax Check (`check_asignation_syntax`)

* **Phase:** Initial pass (`INITIAL\_PHASE`).
* **Logic:** The `check_asignation_syntax` function first verifies that the token's `value` string strictly follows assignment rules:
    * It must contain at least one unquoted **`=`** sign.
    * The variable name (text before the `=`) must begin with an **alphabetic character or underscore (`_`)** and contain only alphanumeric characters or underscores.
* **Action:** If the syntax is valid, the token is provisionally converted from `WORD`/`COMMAND` to **`ASIGNATION`**.

### 4.5.2. External Context Check (`check_externs_syntax`)

* **Phase:** Final pass (`FINAL\_PHASE`).
* **Logic:** This is the critical step. `check_externs_syntax` validates the token's environment by checking its neighbors (tokens to the left and right). A token only retains the `ASIGNATION` type if:
    1.  It is at the start of the command line (`token->id == 0`).
    2.  It is preceded by a **delimiter** (`|`, `&&`, `;`, `(`).
    3.  It is preceded by an **`EXPORT` built-in**.
* **Action:** If the token fails this contextual check (e.g., if it is preceded by a regular `COMMAND` like `ls`), it is definitively reverted back to **`WORD`** using `transform_invalid_asig_to_word`.

**Result:** A token is only considered a definitive, executable assignment if it successfully passes both the internal syntax and external context filters.

---

## 4.6. Transformation Pipeline and Semantic Refinement

The `transform_tokens_logic` function executes a detailed, multi-step pipeline to handle all remaining ambiguities.

### 4.6.1. Assignment Specialization

Once an `ASIGNATION` is confirmed, its type is further refined for execution:

* **Concatenation (`transform_asig_to_asig_plus`):** Assignments containing the `+=` operator (e.g., `VAR+=1`) are converted to the specialized type **`PLUS_ASIGNATION`**. This provides the executor with the precise instruction to append the value rather than replace it.
* **Temporal Context (`transform_asig_to_temp`):** The logic analyzes whether the assignment is meant for the parent shell or a child process.
    * If an assignment is immediately followed by a command or subshell (e.g., `VAR=1 ls -l` or `VAR=1 (cmd)`), it is converted to **`TEMP_ASIGNATION`** or **`TEMP_PLUS_ASIGNATION`**.
    * **Impact:** This ensures the variable is only applied to the child process environment, accurately emulating Bash's behavior.

### 4.6.2. Word and Command Reclassification

Throughout the pipeline, the roles of generic tokens are repeatedly checked against their final context:

| Transformation | Condition & Impact |
| :--- | :--- |
| `transform_cmd_to_word` | **Argument Reversion:** Ensures tokens initially classified as `COMMAND` or `BUILTIN` are downgraded to **`WORD`** if they appear as arguments (i.e., immediately following another command/builtin). This ensures arguments are collected correctly by the AST builder. |
| `transform_word_to_file` | **Redirection Context:** Converts **`WORD`**, `EXPANSION`, or `WILDCARD` tokens that immediately follow a redirection operator (`<`, `>`) into the dedicated **`FILENAME`** type (or `DELIMITER` for `<<`). |
| `transform_word_to_wildcard` | **Wildcard Typing:** Checks for unquoted tokens containing the `*` character and converts them to the **`WILDCARD`** type, flagging them for pattern matching during the execution phase. |
| `transform_cmd_to_built_in` | **Case Normalization:** Performs a final check to confirm if a `COMMAND` is actually a **`BUILTIN`** and converts the value to lowercase for standardized matching. |

This rigorous multi-pass system is vital to stabilize the token array, ensuring that the AST Builder receives a syntactically and semantically unambiguous list of command arguments and operators.

```c
void	transform_tokens_logic(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	transform_cmd_to_built_in(data, prompt, tokens);
	transform_cmd_to_word(data, tokens, INITIAL_PHASE);
	transform_word_to_asignation(data, tokens, INITIAL_PHASE);
	transform_word_to_asignation(data, tokens, FINAL_PHASE);
	transform_cmd_to_word(data, tokens, FINAL_PHASE);
	transform_invalid_asig_to_word(prompt, tokens);
	transform_asig_to_asig_plus(prompt, tokens);
	transform_asig_to_temp(prompt, tokens);
	transform_word_to_file(prompt, tokens);
	transform_word_to_wildcard(prompt, tokens);
	transform_command_built_lowercase(prompt, tokens);
	transform_cmd_to_built_in(data, prompt, tokens);
}
```
---

# 5. ✨ Wildcard Expansion

The Wildcard Expansion Phase is the final array-modifying step in the token processing pipeline. Its role is to take a single **`WILDCARD`** token (containing the `*` pattern) and replace it with a sorted list of matching filenames from the current directory, if any exist.

The process is executed in both the **INITIAL\_PHASE** (to check basic validity) and the **FINAL\_PHASE** (to perform the actual expansion and array modification).

## 6.1. Wildcard Processing Flow (`process_wildcard`)

The expansion of a wildcard token is an intensive, multi-step process that utilizes a temporary structure, **`t_wild`**, to store pattern matching details.

```c
int	process_wildcard(t_shell *data, t_token *token)
{
	char	**new_tokens;
	int		n_dirs;

	new_tokens = NULL;
	n_dirs = 0;
	if (token->double_quoted)
		return (FAILURE);
	init_wildcard(data, &token->wildcard_info);
	if (!extract(data, token, token->wildcard_info))
		return (FAILURE);
	if (!matches(data, token, token->wildcard_info, &n_dirs))
		return (FAILURE);
	new_tokens = find_matches(data, token->wildcard_info, n_dirs);
	if (!new_tokens)
	{
		free_wildcard(&token->wildcard_info);
		return (FAILURE);
	}
	free_wildcard(&token->wildcard_info);
	rebuild_tokens(data, token, new_tokens, n_dirs);
	ft_free_str_array(&new_tokens);
	return (SUCCESS);
}
```

### 5.1.1. Pattern Analysis and Classification

1.  **Initialization:** A temporary **`t_wild`** structure is initialized on the token.
2.  **Extraction (`extract_wildcard.c`):** The token's raw value (e.g., `*.c` or `temp*file`) is analyzed by `extract_wildcard` to determine the pattern type:
    * **`ALL`:** (`*`) - Matches all files.
    * **`BEGINING`:** (`*file`) - Matches files ending with `file`.
    * **`END`:** (`file*`) - Matches files beginning with `file`.
    * **`COMPLEX`:** (`file*part*ext`) - Contains multiple internal wildcards.
3.  **Dotfiles Rule (`should_ignore_file`):** The `t_wild` structure tracks if the pattern explicitly begins with a dot (`.`). This adheres to the Bash rule: if the pattern doesn't start with a dot, ignore files that start with a dot.

### 5.1.2. Matching and Safety Checks

1.  **Count Matches (`count_matches.c`):** The system opens the current directory (`opendir(".")`) and iterates through its entries (`readdir`).
    * **Matching Logic:** For each file, specialized logic checks for a match based on the pattern type (`if_theres_match` for simple patterns; `handle_complex_case` for complex patterns).
    * **Ambiguity Check:** Before array modification, a critical **syntax safety check** is performed: if the wildcard is preceded by a **redirection operator** (e.g., `> *.txt`) and results in **more than one match** (`n_dirs > 1`), an `ERR_AMBIGUOUS_REDIR` syntax error is raised. This is standard Bash behavior.

2.  **Find Matches (`find_matches.c`):** If the count is valid, `find_matches` performs a second directory traversal to allocate and populate a **`char **dirs`** array with the names of all matching files.

### 5.1.3. Array Rebuilding (`rebuild_tokens`)

If matches are found, the single `WILDCARD` token must be replaced by the array of matching filenames.

1.  **Reorder Tokens (`reorder_tokens.c`):** This is the memory-intensive operation that replaces the token:
    * It creates a larger, temporary array (`t_token *tmp`).
    * **Copies Original Tokens** up to the wildcard's position.
    * **Copies New Tokens:** Inserts the matched filenames (`dirs`) as new **`WORD`** tokens into the array.
    * **Copies Remaining Tokens:** Shifts and copies the tokens that followed the original wildcard.
    * **Hash Integrity:** When inserting new tokens, a **unique `hash`** is generated for each using `create_hash` to guarantee no collisions occur with existing tokens, which is essential for AST integrity.
    * The old token array is freed, and the prompt's pointer is updated to the new array.
2.  **Synchronization:** After the array is rebuilt, the tokens' dynamic indices are adjusted (`adjust_id`), and **`reconect_nodes_tokens`** is called to update the pointers in the AST, restoring the execution context.
3.  **No Match Scenario:** If `n_dirs` is zero, the original `WILDCARD` token is left untouched in the array, preserving its literal value for the next phase.

## 5.2. Complex Pattern Matching (`handle_complex_case.c`)

For complex patterns containing multiple internal wildcards (e.g., `*part1*part2*`), a sophisticated matching algorithm is used:

1.  **Pattern Splitting:** The complex wildcard string is split into an array of required literal substrings (e.g., `part1`, `part2`).
2.  **Sequential Match:** The algorithm verifies that **all** literal substrings exist within the target filename *and* appear in the correct sequential order.
3.  **Pointer Comparison:** It uses `ft_intstr_match` to find the starting index of each literal part within the filename and stores these indices in an array (`result`).
4.  **Order Validation:** A final check (`compare_match_order`) ensures that the indices stored in `result` are strictly **ascending**.
    * **Impact:** This guarantees that the matched file (e.g., `a_part1_b_part2_c`) adheres to the strict ordering enforced by Bash for complex wildcard expressions.

# 6. 🌳 Abstract Syntax Tree (AST) Construction

The AST Construction phase translates the simplified, validated token stream into a hierarchical structure that models the logical flow and dependencies of the user's command line. This is achieved using a **Recursive Descent Parser** based on operator precedence.

## 6.1. The Precedence Hierarchy

The parser functions are structured to reflect the standard Unix shell precedence rules. The `ast_builder` function initiates the process by calling the function with the lowest binding strength (`parse_sequence`), ensuring the correct tree structure is built from the top-down.


| Precedence Level | Function | Operators Handled | Role in the Tree |
| :--- | :--- | :--- | :--- |
| **Lowest (1)** | `parse_sequence` | **`;`** (Semicolon) | Groups commands for sequential, unconditional execution. |
| **2** | `parse_and_or` | **`&&`**, **`OR`** (AND, OR) | Groups pipelines based on logical success/failure of the left side. |
| **3** | `parse_pipes` | **`Pipe`** (Pipe) | Groups commands for chained execution, directing output to the next command's input. |
| **Highest (4)** | `parse_subshell` | **`(`**, **`)`** | Handles subshells, real assignments, and delegates to the final command parser. |
| **Leaf Node** | `parse_cmd` | `COMMAND`, `BUILTIN`, `WILDCARD` | Creates the final executable nodes. |

The parser functions follow a standard pattern: they attempt to parse a lower-precedence component as the **left child**, and if they find their operator (e.g., `PIPE`), they create a **central node** with that operator type and recursively parse the **right child**.

## 6.2. Leaf Node Construction: `parse_cmd` and `parse_subshell`

These functions are responsible for creating the executable nodes that form the leaves (or internal wrappers) of the AST.

### 6.2.1. Handling Commands and Assignments (`parse_cmd.c`)

The `parse_cmd` function is versatile, handling not just standard commands but also special cases and implicit actions:

* **Special Cases (`special_cases`):** This helper detects command lines that consist solely of **Redirections** or **Temporary Assignments** (e.g., `VAR=1 > file.txt`). If found, it creates a placeholder command node (a **`true` node**) specifically to execute the redirections and assignments without an explicit executable.
* **Token Inclusion:** It creates the central node for `COMMAND`, `BUILTIN`, `WILDCARD`, or **Local Assignments** (`is_asignation_type`).
* **Wildcards:** Tokens classified as `WILDCARD` or `EXPANSION` are correctly parsed as command nodes. This design choice handles the edge case where an unexpanded wildcard (e.g., `*`) acts as the command itself, deferring the final expansion and error check to the executor.

```c
t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;

	central = NULL;
	central = special_cases(data, tokens, i, n_tokens);
	if (central)
		return (central);
	if (*i < n_tokens && tokens[*i].type
		&& (is_cmd_builtin_type(tokens[*i].type)
			|| is_real_assignation_type(tokens[*i].type)
			|| is_redir_type(tokens[*i].type) || tokens[*i].type == WILDCARD
			|| tokens[*i].type == EXPANSION))
	{
		index_redir_input(tokens[*i].type, i, n_tokens);
		central = create_node(data, &tokens[*i], tokens[*i].type);
		if (!central)
			return (NULL);
		if (is_asignation_type(tokens[*i].type))
			return (safe_index_plus(i, data->prompt.n_tokens), central);
		get_information(data, tokens, i, central);
		if (data->error_state == TRUE || check_signal_node_heredoc(central))
			return (clean_node(&central), NULL);
		return (central);
	}
	return (central);
}
```

### 6.2.2. Handling Subshells and Assignment Sequences (`parse_subshell.c`, `parse_assignations.c`)

* **Subshells:** If `parse_subshell` encounters a **`PAREN_OPEN`** token, it creates a **`SUBSHELL`** node. It recursively calls `parse_sequence` to build the entire command structure **inside** the parentheses as its left child.
* **Assignments:** If the current token is a **Real Assignment** (`ASIGNATION`, `PLUS_ASIGNATION`), `parse_assignations` is called. This function groups multiple consecutive assignments by chaining them together with **`;` (SEMICOLON) nodes**.
    * **Example:** `VAR=1 VAR+=2` is converted into an AST structure equivalent to `VAR=1 ; VAR+=2`, ensuring they are executed sequentially.

## 6.3. Node Information Assembly (`get_information`)

After the central command node is created, `get_information` gathers all peripheral metadata required for execution.

### 6.3.1. Temporary Assignments (`get_temp_asignations.c`)

* **Context Scoping:** Temporary assignments (e.g., `VAR=1 ls -l`) only apply to the command they precede.
* **Logic:** `get_temp_asignations` searches backward from the command's index to locate and extract tokens flagged as `TEMP_ASIGNATION`. It builds a **`char **assig\_tmp`** array on the command node.
* **Impact:** This metadata is crucial for the executor, which must apply these environment variables only to the child process executing the current command.

### 6.3.2. Redirections and Heredocs (`get_redirs.c`, `get_heredoc.c`)

* **Redirection List:** `get_redirs` iterates through the command's token range, identifies all redirection operators (`<`, `>`, `>>`, `<<`), and builds a linked list of **`t_redir`** structures on the node's `redir` member.
* **Heredoc Management:** If a `REDIR_HEREDOC` (`<<`) is found:
    1.  `get_heredoc` is called to enter the prompt loop (`loop_heredoc`).
    2.  The user's input lines are collected and stored in a linked list (`t_list *heredoc_lines`) within the `t_redir` structure.
    3.  **Expansion Rule:** `expand_heredoc` checks if the delimiter itself was quoted. If the delimiter was **unquoted**, the `redir->expand` flag is set to `TRUE`, indicating that variables within the collected lines must be expanded later during execution.

### 6.3.3. Arguments and Context Types

* **Binary Arguments (`get_args_for_binary.c`):** This function iterates forward from the command token to collect all subsequent `WORD` tokens (excluding redirections, temporary assignments, and delimiters). It builds the standard **`char **args`** array for the executor (e.g., `{"ls", "-l", NULL}`).
* **Argument Types (`get_arg_types.c`):** For specialized built-ins like `export`, the executor needs to know the context of each argument. `get_arg_types` creates an auxiliary array (`int *arg\_types`) that stores the **original token `id`** for each argument.
    * **Impact:** This preserves the original semantic type (e.g., `ASIGNATION`, `PLUS_ASIGNATION`) for `export` arguments, allowing the built-in logic to correctly process and update the environment.

### 6.3.4. Background Operator (`get_background.c`)

* The parser checks if the final token after all arguments and redirections is a **`BACKGROUND`** operator (`&`). If present, it sets the node's `background` flag to `TRUE` and advances the index past the operator.

-----

# 7\. 🚀 AST Execution Phase

The Execution Phase is the runtime core of Minishell. It traverses the Abstract Syntax Tree (AST) constructed by the parser, interprets the logical structure (pipes, AND/OR, subshells), and manages process creation (`fork`), I/O redirection, and signal handling to execute commands and built-ins.

## 7.1. Recursive Executor and Precedence

The main function, **`executor_recursive`**, traverses the AST starting from the root node. It delegates the execution based on the node's type, strictly following the precedence defined by the AST structure.

```c
void executor_recursive(t_shell *data, t_node *node, t_exec *exec, int mode)
{
    // ... node->executed = true; ...
	if (node->type == SEMICOLON)
		exec_semicolon(data, node, exec, mode);
	else if (node->type == PIPE)
		exec_pipe(data, node, exec, mode);
    // ... other node types ...
}
```

### 7.1.1. Logical Operators and Sequence

Execution for logical and sequence nodes is straightforward, utilizing the exit code (`data->exit_code`) to control flow:

| Node Type | Execution Function | Logic |
| :--- | :--- | :--- |
| **`SEMICOLON`** (`;`) | `exec_semicolon` | Executes the left child, then the right child, regardless of the exit code. (Sequential Execution) |
| **`AND`** (`&&`) | `exec_and` | Executes the left child. Only executes the right child if `data->exit_code == 0` (Success). |
| **`OR`** (`||`) | `exec_or` | Executes the left child. Only executes the right child if `data->exit_code != 0` (Failure). |

## 7.2. Command and Built-in Execution Modes

Minishell uses a precise system to determine the execution context based on the current mode (`FATHER`, `CHILD`, `SUBSHELL`).

| Execution Mode | Context | Process Creation (`fork`) |
| :--- | :--- | :--- |
| **`FATHER`** | Top-level command or command after a sequence operator (`&&`, `||`, `;`). | **Required:** A new child process is forked (`execute_cmd_from_father`). |
| **`CHILD`** | Command is part of a **Pipe** (`|`). | **Not Required:** Execution occurs directly in the child process already forked by `exec_pipe`. |
| **`SUBSHELL`** | Command is part of a `SUBSHELL` node. | **Required:** A new child process is forked by `exec_subshell`. |

### 7.2.1. Property Application (`apply_properties`)

Before any command or built-in is executed (in the parent or child process), **`apply_properties`** is called to set up the necessary environment:

1.  **Temporary Assignments:** If the node has `assig_tmp` (temporary assignments like `VAR=1 ls`), **`apply_temp_asig`** adds them to the child's environment variables.
2.  **Redirections:** If the node has `redir` structures, **`apply_redirs`** handles file opening (or pipe duplication) and redirects standard file descriptors (`STDIN`, `STDOUT`, `STDERR`) using `dup2`.

## 7.3. Command Execution (`exec_command.c`)

This is the path for external executables found in `PATH` (e.g., `/bin/ls`) or commands that require a full search.

1.  **Final Transformation:** The crucial **`final_expansion_process`** is called first. This executes all deferred expansions, word splitting, and wildcard expansion specifically for the current command's arguments.
2.  **Process Control:** Based on `mode`, either `execute_cmd_from_father` (if `mode == FATHER`) or `execute_cmd_from_child` (if `mode == CHILD`) is called.
      * **Child Process Logic:** The child process: 1) calls `apply_properties`, 2) finds the executable path (`get_path`), 3) updates the `_` environment variable, and 4) executes the command using **`execve`**.
3.  **Background Control (`wait_cmd_background`):**
      * If `node->background` is **FALSE**, the parent process calls `waitpid` to wait for the child's termination and retrieves the final exit status.
      * If `node->background` is **TRUE** (`&` operator), the parent prints the PID and **does not wait**, returning immediately to the interactive shell.

## 7.4. Built-in and Assignment Execution (`exec_builtin.c`)

Built-in commands (like `cd`, `export`, `exit`) are handled separately because they must modify the state of the **parent shell process**.

1.  **Foreground Execution:** If the node is *not* in the background, the built-in is executed **directly in the current process** (`FATHER` or `SUBSHELL` process) without a `fork`. This ensures changes to the environment (`export`, `unset`) or shell state (`cd`, `exit`) persist.
2.  **Background Execution:** If `node->background` is **TRUE**, a `fork()` is required (`hanlde_background_exec`). The built-in is executed in the child process, and the parent prints the PID and continues. This sacrifices persistence (e.g., `cd` in background won't affect the parent) but preserves process control.
3.  **Built-in Selection:** The function **`which_builtin`** delegates control to the correct internal function (e.g., `my_export`, `my_cd`, `my_echo`) based on the token's value.

## 7.5. Pipelining and Subshells

### 7.5.1. Pipe Execution (`exec_pipe.c`)

Pipe nodes (`|`) execute commands concurrently using standard Unix pipelining:

1.  **Pipe Creation:** `pipe(pipefd)` creates the pipe.
2.  **Forking:** Two child processes are forked, one for the left command and one for the right command.
3.  **I/O Duplication (`handle_child`):**
      * **Left Child:** Duplicates the write end of the pipe (`pipefd[1]`) to `STDOUT_FILENO`.
      * **Right Child:** Duplicates the read end of the pipe (`pipefd[0]`) to `STDIN_FILENO`.
4.  **Waiting:** The parent process closes the pipe descriptors and uses `waitpid` to wait for both children, setting the final exit code based on the **rightmost** command's status.

### 7.5.2. Subshell Execution (`exec_subshell.c`)

Subshells (`(cmd1 | cmd2)`) force the enclosed command structure to run in a separate process:

1.  **Forking:** A single child process is forked.
2.  **Execution:** The child process applies redirections and recursively calls `executor_recursive` on the subshell's content (`node->left`).
3.  **Exit:** The child process terminates using `exit_succes` with the exit code resulting from the sub-command execution.
4.  **Waiting:** The parent waits for the subshell process, retrieves the final status, and handles any signal termination.

---

# 7.6. 📤 I/O Management: Redirections and Heredocs

The shell's I/O management system is responsible for implementing the behavior of all redirection operators ($\lt$, $\gt$, $\gt\gt$, $\lt\lt$). This logic is housed primarily within the **`apply_redirs`** function, which is called before any command execution takes place.

### 7.6.1. Redirection Application Flow (`apply_redirs`)

The `apply_redirs` function iterates through the linked list of **`t_redir`** structures attached to the current command node. For each redirection, it performs the following:

1.  **Ambiguity Check:** It calls `check_ambiguous_redir` (not detailed here, but essential) to ensure the filename (often resulting from expansion) does not resolve to multiple files, which would constitute an ambiguous redirect error.
2.  **File Descriptor Handling:** Based on the redirection type (`type`), it calls the appropriate handler function to open the target file and duplicate the relevant file descriptor (`dup2`).
3.  **Error Propagation:** If a file opening fails (due to permissions, `EACCES`, or file not found, `ENOENT`), the function prints the specific error message and returns `FAILURE`. If this occurs in a **child process** (`mode == CHILD`), the child terminates immediately via `exit_error`.

#### **Standard Redirection Handlers:**

| Type | Function | `open` Flags | Action |
| :--- | :--- | :--- | :--- |
| **`REDIR_OUTPUT`** ($\gt$) | `handle_redir_output` | `O_WRONLY | O_CREAT | O_TRUNC` | Opens file for writing, creates it if missing, and **truncates** (clears) existing content. Redirects to `fd_redir` (default $\text{STDOUT}$). |
| **`REDIR_APPEND`** ($\gt\gt$) | `handle_redir_append` | `O_WRONLY | O_CREAT | O_APPEND` | Opens file for writing, creates it if missing, and **appends** new data to the end. Redirects to `fd_redir`. |
| **`REDIR_INPUT`** ($\lt$) | `handle_redir_input` | `O_RDONLY` | Opens file for reading. Redirects to `fd_redir` (default $\text{STDIN}$). |

### 7.6.2. Heredoc Execution ($\lt\lt$)

The **Heredoc** operator requires dynamic I/O creation to pass multi-line user input to the command.

#### **Heredoc Execution Logic (`handle_redir_heredoc`)**

1.  **Pipe Creation:** A temporary **pipe** (`pipe(pipe_fd)`) is created. This pipe serves as the virtual file that will hold the heredoc content.
2.  **Content Expansion and Writing:** The function iterates through the lines previously collected from the user (`redir->heredoc_lines`).
    * **Conditional Expansion:** If the original delimiter was **unquoted** (`redir->expand == TRUE`), the line is sent to **`expand_line_heredoc`** for environment variable substitution *before* being written.
    * Each line is written sequentially to the **write end** of the pipe (`pipe_fd[1]`).
3.  **I/O Redirection:**
    * The write end (`pipe_fd[1]`) is closed after all content is written.
    * The read end (`pipe_fd[0]`) is duplicated onto **`STDIN_FILENO`** using `dup2`.
4.  **Cleanup:** The temporary read end of the pipe is closed. The command process now reads its input directly from the pipe, which contains the buffered heredoc text.

#### **Heredoc Line Expansion (`expand_line_heredoc.c`)**

This function performs variable substitution specifically on the lines collected within the heredoc:

1.  The input line is treated as a temporary token (`t_token`).
2.  It utilizes a simplified version of the main expansion logic to find and substitute **`$` variables** (e.g., `$USER`) and **special variables** (e.g., `$?`) within the line.
3.  The expanded line is returned and subsequently written to the pipe. This confirms that, unlike the main shell expansion, **heredoc expansion is a single-step, line-by-line substitution without complex features like word splitting or array reorganization.**

---

# 8. 🎯 Assignment Engine: Scope, Persistence, and State Management

The Assignment Engine is the subsystem responsible for parsing assignment tokens, validating their syntax and context, determining their scope (Local, Exported, or Temporary), and managing how their values interact with the shell's environment (`t_var` linked list).

## 8.1. Assignment Execution Pipeline (`asignation.c`)

The central function, **`asignation(t_shell *data, t_token *token, int type)`**, orchestrates the process of converting a token into a persistent environment variable.

### 8.1.1. Key and Value Extraction

1.  **Memory Allocation:** Dedicated memory buffers are allocated for the `key` (variable name) and `value` (variable content).
2.  **Extraction:** Utility functions (`aux_key_asig`, `aux_value_asig`) carefully extract the key and value from the raw token string (`token->value`):
    * **Key Extraction:** Stops at the first unquoted `=` sign, while accounting for the `+` in `+=`.
    * **Value Extraction:** Collects all characters after the `=` sign.
    * **Impact:** This step separates the semantic components needed for environment manipulation.

### 8.1.2. State Verification and Update (`verify_if_already_set.c`)

Before creating a new variable, the system must check if the variable already exists using **`verify_if_already_set`**.

* **Lookup:** The environment linked list (`data->env.vars`) is traversed using `ft_strcmp` on the `key`.
* **Update Logic (`handle_existing_value`):** If the variable is found, its value and type are updated based on the assignment mode (`t`):
    * **Standard Assignment (`LOCAL`, `ENV`, etc.):** The old value is freed, and the new value is stored.
    * **Concatenation (`PLUS_ASIGNATION`):** **`handle_plus_assignation`** concatenates the new value onto the existing value using `ft_strjoin`.
    * **Type Promotion:** If a variable currently marked as **`LOCAL`** is assigned via an **`EXPORT`** command, its type is promoted to **`ENV`** (`update_variable_type`), ensuring it persists in the child processes.
* **Result:** If the variable exists and is updated, the function returns **`TRUE`**. If not found, it returns **`FALSE`**, triggering the creation of a new variable.

### 8.1.3. New Variable Creation

If the variable does not exist, **`add_var_and_envp`** is called to create a new `t_var` node and append it to the environment list.

* **Type Determination:** If the original token type was **`PLUS_ASIGNATION`** and the variable is new, `is_it_env_or_local` performs a backward search to determine if the assignment belongs to an `EXPORT` context (`ENV`) or a general command line context (`LOCAL`). This resolves the ambiguous nature of a new `+=` assignment.

## 8.2. Assignment Scope and Persistence

Minishell meticulously tracks variable scope to emulate Bash's environment persistence rules.

| Type | Context | Persistence | Example |
| :--- | :--- | :--- | :--- |
| **`LOCAL`** | `VAR=value` at the start of the line or after a delimiter without `export`. | **Parent Shell Only.** Used for internal shell variables; not passed to `execve`. | `MYVAR=1; echo $MYVAR` |
| **`ENV`** | `export VAR=value` | **Exported.** Passed to child processes and visible in `env`. | `export MYVAR=1` |
| **`TEMP\_ASIGNATION`** | `VAR=value cmd` | **Temporary.** Applied only to the environment of the *immediate child process* executing `cmd`, then cleaned up in the parent. | `TEMP=1 ls` |
| **`PLUS\_ASIGNATION`** | `VAR+=value` (concatenation) | Handled by `handle_plus_assignation` to append values. | `VAR=a; VAR+=b` |

### 8.2.1. Handling Temporary Assignments

The **`transform_asig_to_temp`** logic handles the transition to temporary assignments, which is crucial for managing the environment stack during execution:

* If an assignment is followed by a **command**, **built-in (non-export)**, or a **subshell**, its type is converted to **`TEMP_ASIGNATION`**.
* During execution, the `exec_command` and `exec_builtin` functions use `clean_temp_variables` to remove these variables from the parent shell's environment immediately after the child process finishes, ensuring the parent's environment remains unchanged.

## 8.3. Syntactic Validation (`check_asignation_syntax.c`)

Before an assignment is processed, its structure must be validated to ensure it adheres to shell naming conventions.

| Rule | Function | Description |
| :--- | :--- | :--- |
| **Valid Key Name** | `check_invalid_char` | The variable key must start with a letter or `_`. It can only contain alphanumeric characters or `_`. |
| **Presence of `=`** | `count_syntax` | Ensures the token contains at least one `=` and that there is text preceding it. |
| **Export Syntax** | `check_invalid_char_exp` | A simplified check for the format `export VAR` (Type `EXP`), ensuring no illegal characters are present throughout the string.
```c
int	asignation(t_shell *data, t_token *token, int type)
{
	char	*key;
	char	*value;
	int		result;
	int		i;

	key = NULL;
	value = NULL;
	i = 0;
	if (aux_mem_alloc_asignation(&key, &value, ft_strlen(token->value)) == ERROR)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	aux_key_asig(token, &key, &i);
	aux_value_asig(token, &value, &i);
	result = verify_if_already_set(data, key, &value, type);
	if (result == TRUE || result == IGNORE)
	{
		free (key);
		free (value);
	}
	else if (result == FALSE)
	{
		if (type == PLUS_ASIGNATION)
			is_it_env_or_local(data, &type, token->id);
		add_var_and_envp(data, key, value, type);
	}
	return (0);
}
```
# 🤯 The Challenge: The Assignment Engine

The Assignment Engine is arguably the hardest *extra* feature of the Minishell project because it requires managing the shell's **persistent global state** while simultaneously respecting **transient, local process environments**. Unlike simple command execution (which relies on `fork/execve`), assignments force the shell to become a data manager with complex scoping rules.

### The Difficulty

1.  **Scope Interception:** The shell must intercept a token (`VAR=value`) and decide its destiny *before* it is ever seen as a command argument. This requires two-tiered validation: 
    * **Syntactic Complexity:** Determining if a token like `VAR+=1` or `VAR=1` is correctly formatted (`check_asignation_syntax`).
    * **Contextual Dependency:** Determining if the token's neighbors permit it to be an assignment (e.g., must not follow an unexported command), which is handled by the **`check_externs_syntax`** logic.
2.  **Environment State Integrity:** Changes made by built-ins (`export`, `unset`) must modify the parent process's state. Failure to execute these directly in the parent would prevent the changes from persisting.
3.  **The Temporary Assignment Flaw:** This is the highest hurdle. The shell must distinguish between:
    * **`export VAR=1`**: Persists everywhere.
    * **`VAR=1`**: Persists only in the parent shell (LOCAL).
    * **`VAR=1 ls`**: Persists *only* for the `ls` child process (`TEMP_ASIGNATION`).

The solution, which involves flagging variables as **`TEMP`** and ensuring they are **cleaned up** immediately after the child process finishes, is essential for correct emulation of Bash's environment inheritance.

```c
int	verify_if_already_set(t_shell *data, char *key, char **value, int t)
{
	t_var	*var;
	int		result;

	result = 0;
	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			result = handle_existing_value(data, var, value, t);
			if (result == IGNORE)
				return (IGNORE);
			else if (result == ERROR)
			{
				free (key);
				free (*value);
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			}
			update_variable_type(var, t);
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
}
```
---

# 8.2. 🗃️ Detailed Breakdown of Assignment Types

Minishell uses seven distinct variable types (including two for expansion purposes) to accurately track variable state, scope, and modification intent within the environment linked list.

| Type | Context/Purpose | Persistence Scope | Modification |
| :--- | :--- | :--- | :--- |
| **`ENV`** | Created by `export VAR=value`. | **Persistent.** Visible to the parent shell and inherited by all child processes via `execve`. | Standard assignment (`=`). |
| **`LOCAL`** | `VAR=value` at the start of the line, outside of `export`. | **Parent Shell Only.** Used by the shell internally; not automatically passed to children via `execve`. | Standard assignment (`=`). |
| **`EXP`** | Created by `export VAR` (without a value). | **Exported, but Value is NULL.** Marks the variable for export, but it holds no value until assigned. | Used for initial setup in the `export` built-in. |
| **`PLUS\_ASIGNATION`** | Used internally by the engine to flag the intent to **concatenate** (`+=`). | Not a storage type; it's a **behavioral flag** used by `verify_if_already_set` to trigger string appending (e.g., `VAR=a; VAR+=b` results in `VAR=ab`). | Concatenation (`+=`). |
| **`TEMP\_ASIGNATION`** | Identified as `VAR=value cmd` (precedes an external command). | **Transient/Child-Specific.** Added to the child's environment before `execve` and **removed immediately** upon return to the parent shell. | Standard assignment (`=`). |
| **`TEMP\_PLUS\_ASIGNATION`** | Identified as `VAR+=value cmd`. | **Transient/Child-Specific.** Concatenates the value and applies it only to the child's environment. | Concatenation (`+=`). |

### Key Scoping Logic

* **Promoting Variables:** If a variable is currently `LOCAL` but is subsequently assigned using `export`, its type is automatically promoted to **`ENV`** (`update_variable_type`), reflecting its new persistent scope.
* **Cleaning Temporary State:** The `exec_command` and `exec_builtin` functions are responsible for calling `clean_temp_variables` after a child process exits. This cleanup step is vital: it iterates through the environment list and removes all variables flagged as `TEMP_ASIGNATION` or `TEMP_PLUS_ASIGNATION`, restoring the parent environment's original state.

---

## 9. 🛠️ Built-in Command Execution

Minishell's built-in commands are vital for core shell functionality and environment management. Unlike external commands, they are executed directly within the running shell process (`exec_builtin`), ensuring their effects (e.g., changing directory or environment variables) are persistent.

## 9.1. 📤 `export` (`my_export.c`)

The `my_export` function orchestrates argument processing, scope promotion, and conditional variable listing.

### The Execution Flow

1.  **Conditional Listing:** If the command is called without arguments (i.e., `node->arg_types` is `NULL`), `my_export` immediately calls `print_env_variables` to iterate over the entire environment list (`t_env`) and output only variables flagged as **`ENV`** or **`EXP`** in the `declare -x` format.
2.  **Argument Processing Loop:** If arguments are present, the function loops through the **`node->arg_types`** array, which contains the indices of the arguments in the main token array.
3.  **Assignment Delegation:** For each argument index, it calls `asignation_type` to delegate the variable creation or update based on the token's classification.
4.  **Context Boundary Check:** The loop includes a critical `check_for_valid_args` check to ensure processing stops immediately if it encounters a delimiter (`PIPE`, `AND`, `OR`, `PAREN_OPEN`). This respects the AST's logical boundaries.

### Precise Variable Typing and Flow

The `export` flow determines the variable's ultimate persistence and value mechanism via the central **`asignation`** function, delegating the **`type`** parameter:

| Token Type | Assignment Delegation | Persistence Flow |
| :--- | :--- | :--- |
| **`ASIGNATION`** (`VAR=value`) | Type **`ENV`** | **Promotion:** The variable is added/updated in the environment and flagged as **`ENV`**, ensuring it is passed to all future child processes via `execve`. |
| **`WORD`** (`VAR`) | Type **`EXP`** | **Exported but Unset:** The variable is added to the environment list and flagged as **`EXP`**. It holds a `NULL` value until a standard assignment occurs later, but its status as an exported variable is secured. |
| **`PLUS_ASIGNATION`** (`VAR+=value`) | Type **`PLUS_ASIGNATION`** | **Concatenation:** The assignment engine uses the `handle_plus_assignation` logic in `verify_if_already_set` to append the new value to the existing variable's content. The final variable type is promoted to **`ENV`**. |

### 🛠️ The Role of `arg_types`

The **`node->arg_types`** array is an engineering solution for managing arguments in the AST.

* **Function:** It is an array of integers that stores the **dynamic index (`id`)** of each argument token relative to the main token array.
* **Necessity:** For `export`, arguments are not just simple words; they must be **`ASIGNATION`** or **`WORD`** tokens whose original syntax and value must be retrieved. By storing the index, `my_export` can efficiently look back into the primary token array to access the exact `token->value` (e.g., the string `"MYVAR=10"` or `"MYVAR"`) and its semantic type.

### Wildcard Expansion within `export`

Wildcard tokens within `export` arguments require specialized, in-place processing to ensure shell rules are followed:

1.  **Pre-check:** If a token is detected as a **`WILDCARD`** (e.g., `export *VAR=value`), the function calls **`expand_wildcards`** on that specific token.
2.  **In-Place Expansion:** `expand_wildcards` replaces the single wildcard token with a list of zero or more matching `WORD` tokens (if any matches are found).
3.  **Post-Expansion Check:** The resulting tokens (which may be a list of filenames or the original unexpanded string) are then immediately checked again for valid assignment syntax using `check_asignation_syntax`.
4.  **Error Handling:** If the expanded result fails the assignment syntax check, an `ERR_EXPORT` is printed, but processing continues for other arguments. This complex flow ensures the export command handles dynamic string generation correctly.

### 9.2. `cd`: Changing Directories and State Update (`my_cd.c`)

The `cd` (change directory) command is one of the most state-intensive built-ins, requiring careful management of path and environment variables.

#### Core Logic:

* **Argument Validation:** `my_cd` first checks that it receives exactly one valid argument (or zero, for HOME).
* **Path Resolution:**
    * If called without arguments, it defaults to the path stored in the **`HOME`** environment variable.
    * If called as `cd -`, it attempts to move to the previous directory stored in **`OLDPWD`**.
* **Directory Validation (`validate_and_move`):** Before calling the system function `chdir()`, it performs rigorous checks:
    1.  **Existence:** `stat()` confirms the target path exists.
    2.  **Type:** `S_ISDIR()` confirms the path points to a directory, not a file.
    3.  **Permissions:** `access()` confirms the shell has execute permission (`X_OK`).
* **State Update (Critical):** After a successful `chdir()`:
    1.  The value of the existing **`PWD`** variable is copied to **`OLDPWD`** (using the path *before* the move).
    2.  The current working directory is retrieved using **`getcwd()`** and is used to update the new value of **`PWD`**. This ensures the shell's internal environment accurately reflects the actual filesystem location.

### 9.3. `unset`: Environment Deletion (`my_unset.c`)

The `unset` built-in permanently removes variables from the shell's environment.

#### Core Logic:

* **Argument Validation:** It checks that argument names adhere to variable naming rules (letters/underscore followed by alphanumeric/underscore).
* **Deletion:** For each valid argument name, it calls **`delete_var`** on the environment linked list (`t_env`). This function is responsible for safely removing the corresponding `t_var` node, updating the `prev` and `next` pointers, and freeing the associated memory (key and value).
* **Impact:** Deleting the node removes the variable from the parent shell's memory, ensuring it is no longer visible to subsequent commands or inherited by future child processes.

Certainly. Let's dive deeper into the core logic of the `export` built-in and the crucial environment cleanup utility, `my_clean_unset`, highlighting how they manage the shell's state and memory.

---

## 9.4. 🗑️ `my_clean_unset`: Temporary Environment Cleanup

The **`my_clean_unset`** function is a non-user-facing, internal utility critical for enforcing **process isolation** and the **Transient Scope** rules of temporary assignments. It ensures that temporary variables do not pollute the parent shell's environment.

### Engineering Rationale: Preventing Leaks

When the shell executes a temporary assignment (e.g., `VAR=1 ls`):
1.  `VAR=1` is correctly added to the child's environment before `ls` runs.
2.  The parent shell, however, must ensure that `VAR=1` is removed from its own environment immediately after the `ls` command finishes. If it didn't, `VAR` would persist as a "leak."

### Core Cleanup Logic:

The function operates on the list of temporary assignments collected on the command node (e.g., `node->assig_tmp`) and works in conjunction with the `my_unset` logic.

1.  **Iteration on Temp Tokens:** `my_clean_unset` iterates through all tokens that were identified as **`TEMP_ASIGNATION`** or **`TEMP_PLUS_ASIGNATION`** for the recently executed command.
2.  **Key Extraction:** For each temporary token, it carefully extracts the variable **key** (the name before the `=` or `+` sign).
    * This is achieved by searching for the delimiter (`=` or `+`) within the token's value and copying the substring before it.
3.  **Runtime Deletion:** It calls the core deletion function **`delete_var`** on the parent shell's global environment list using the extracted key.
4.  **Impact:** This action enforces the **transient nature** of the variable. By calling `delete_var` after the child process (or built-in) execution, the shell guarantees that the variable only existed for the duration of that single command, keeping the parent shell's environment clean and stable.

This entire mechanism guarantees that variables flagged as `TEMP` will not affect the global state, ensuring a **clean and logical flow** that respects the parent shell's environment integrity.
---

## 9.5. Summary of Other Built-ins

The remaining built-in commands handle simpler tasks related to I/O and shell termination:

| Built-in | Function | Core Logic |
| :--- | :--- | :--- |
| **`echo`** | `my_echo` | Prints arguments to `STDOUT`. Implements special logic to detect and handle the **`-n` flag** and its variations (e.g., `-nnnn`), suppressing the final newline character if found. |
| **`pwd`** | `my_pwd` | Prints the current working directory. Primarily uses the system call `getcwd()`. It includes fallback logic to use the `PWD` environment variable if `getcwd()` fails (e.g., if the current directory was deleted). |
| **`env`** | `my_env` | Prints the environment variables. Iterates through the `t_var` list and prints only variables marked with the **`ENV`** type in the `KEY=VALUE` format. It fails if any arguments are provided. |
| **`exit`** | `my_exit` | Terminates the shell. Validates the number of arguments (must be 0 or 1). If 1 argument is provided, it verifies the argument is a valid **numeric value** and uses it as the exit status (modulo 256). Sets the shell's exit status and calls `exit_succes` for final termination.

```c
static void	asignations(t_shell *data, t_token *token)
{
	if (token->type == ASIGNATION)
		data->exit_code = asignation(data, token, LOCAL);
	else if (token->type == PLUS_ASIGNATION)
		data->exit_code = asignation(data, token, PLUS_ASIGNATION);
}

static void	env_cmds(t_shell *data, t_env *env, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		data->exit_code = my_export(data, data->prompt.tokens, env, node);
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		data->exit_code = my_unset(data, env, node->args);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		data->exit_code = my_env(env->vars, node->args);
}

static void	basic_builtins(t_shell *data, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->exit_code = my_echo(node->args);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		data->exit_code = my_pwd(data);
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit(data, node->args);
	else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
		data->exit_code = my_cd(data, node->args);
}

void	which_builtin(t_shell *data, t_token *token, t_node *node)
{
	asignations(data, token);
	env_cmds(data, &data->env, token, node);
	basic_builtins(data, token, node);
}
```
---
This final file, `minishell_structs.h`, provides the comprehensive definitions for all the structures and enumerations used throughout your Minishell project. This information is crucial for understanding the data model that underpins the entire execution flow.

Here is the detailed documentation for the core data structures, organized by their function in the Minishell pipeline.

---

# 10. 🧬 Minishell Data Model: Core Structures and Types

The `minishell_structs.h` file defines the persistent and transient data structures that manage the shell's state, input processing, and execution context.

## 10.1. Lexical and Semantic Types (`enum e_type`)

The `t_type` enumeration is the backbone of the shell's semantic system. Every token, variable, and AST node is categorized by one of these types, guiding the parser and executor logic.

### Structural and Operator Types
* **Logical/Sequence:** `SEMICOLON`, `AND`, `OR`, `PIPE`.
* **Grouping:** `PAREN_OPEN`, `PAREN_CLOSE`, `SUBSHELL`.
* **Redirection:** `REDIR_INPUT`, `REDIR_OUTPUT`, `REDIR_APPEND`, `REDIR_HEREDOC`.
* **Metacharacters:** `BACKGROUND`, `WILDCARD`, `EXPANSION`.

### Executable and Word Types
* **Commands:** `COMMAND`, `BUILT_IN`, `SCRIPT_ARG`.
* **General Content:** `WORD`, `FILENAME`, `DELIMITER` (Heredoc delimiter).

### Assignment and Scope Types (Critical for State Management)
* **Local/Exported:** `ASIGNATION`, `LOCAL`, `ENV`, `EXP` (Exported, Unset).
* **Concatenation:** `PLUS_ASIGNATION`.
* **Temporary/Transient:** `TEMP_ASIGNATION`, `TEMP_PLUS_ASIGNATION`.

### Utility and Internal Types
* `NO_SPACE`: Used by the Tokenizer to flag mandatory token concatenation.
* `DONT_ELIMINATE`: Used by the Simplifier to override concatenation rules.
* `INDIFERENT`, `DELETE`, `NEW_TOKEN_TO_ORGANIZE`: Internal markers for cleanup and array reorganization.

## 10.2. Pipeline Data Structures

These structures manage the data flow from input to final execution.

### A. Token Structure (`struct s_token`)
The atomic unit of the parser.
* **`id`** (`int`): **Dynamic Index.** The current index in the token array, updated frequently during array modification (simplification, expansion).
* **`hash`** (`int`): **Permanent Identifier.** A fixed value used to maintain the link to the corresponding AST node (`t_node`) throughout array reorganization.
* **`type`** (`t_type`): The final semantic role of the token (e.g., `COMMAND`, `WILDCARD`).
* **`value`** (`char *`): The final, cleaned string content.
* **`single_quoted` / `double_quoted`** (`bool`): Flags indicating the original quoting context, crucial for deferred expansion rules.

### B. Prompt Structure (`struct s_prompt`)
Manages the input and the dynamic token array.
* **`input`** (`char *`): The raw command line string read from the user.
* **`tokens`** (`t_token *`): The dynamically allocated array holding all generated tokens.
* **`n_tokens` / `n_alloc_tokens`** (`int`): Counters for the current number of tokens and the allocated capacity (used for dynamic resizing).
* **`before_tokens_type`** (`int *`): Array used to save the original `t_type` of tokens before expansion, necessary for conditional word splitting (refer to Section 3.5).

### C. Environment Variable Structure (`struct s_var`)
The node structure for the environment linked list.
* **`key` / `value`** (`char *`): The variable name and its assigned value.
* **`type`** (`t_type`): The persistence scope of the variable (`ENV`, `LOCAL`, `EXP`).
* **`next` / `prev`** (`t_var *`): Pointers for the doubly linked list, enabling efficient insertion and deletion by `export` and `unset`.

## 10.3. Execution and Control Structures

These structures define the command structure and the shell's global execution state.

### A. AST Node Structure (`struct s_node`)
The building block of the Abstract Syntax Tree (AST).
* **`type`** (`t_type`): The operator or command type (e.g., `PIPE`, `COMMAND`, `SUBSHELL`).
* **`left` / `right`** (`t_node *`): Pointers defining the hierarchical relationship of the AST.
* **`token`** (`t_token *`): A pointer to the primary token (e.g., the command name or the pipe symbol) that the node represents.
* **`token_hash`** (`int`): Stores the permanent hash of the token for link restoration after array modification.
* **`args`** (`char **`): The final argument vector (`argv`) passed to `execve`.
* **`assig_tmp`** (`char **`): Array of transient assignments (`TEMP_ASIGNATION`) applied only to this node's execution context.
* **`redir`** (`t_redir *`): Linked list of all I/O redirections associated with this command.
* **`background`** (`bool`): Flag set if the command should run in the background (`&`).

### B. Redirection Structure (`struct s_redir`)
Details for each I/O redirection operation.
* **`type`** (`t_type`): The operator type ($\lt, \gt, \lt\lt$, etc.).
* **`filename`** (`char *`): The target file path.
* **`fd_redir`** (`int`): The file descriptor to be redirected (e.g., 0 for stdin, 1 for stdout, or an explicit number like `2>`).
* **`heredoc_lines`** (`t_list *`): A linked list storing the collected input lines for `REDIR_HEREDOC`.
* **`expand`** (`bool`): Flag to indicate if variables in the heredoc content should be expanded.

### C. Shell State Structure (`struct s_shell`)
The master structure containing all global state.
* **`env`** (`t_env`): The structure managing the environment variables list and `envp` array.
* **`prompt`** (`t_prompt`): The structure managing the current input and tokens.
* **`ast_root`** (`t_node *`): Pointer to the root of the active AST.
* **`exec`** (`t_exec`): Contains original standard file descriptors (`stdin`, `stdout`) for restoration after redirection.
* **`exit_code`** (`int`): Stores the exit status of the last executed command ($`?`).
* **`error_state`** (`bool`): A flag used internally to signal critical errors that should stop the execution flow.

---

# 11. ✨ Extra Features: UX and Robustness

## 11.1. ⚖️ Multi-line Input Handling: Balancing the Prompt

A core robustness feature is the shell's ability to recognize incomplete commands (unbalanced quotes, unclosed parentheses, etc.) and seamlessly prompt the user for continuation until the command is logically complete.

https://github.com/user-attachments/assets/c1c2fbdc-e16c-49e4-a81c-50a36a79bd65

### The Balancing Engine (`read_until_balanced.c`)

1.  **Initial Check:** The function `read_until_balanced` takes the initial input line and passes it to the `check_global_balance` function.
2.  **Global Balance Audit:** `check_global_balance` delegates the task to several sub-functions that check specific syntactic components:
    * **Quotes:** Checks for unmatched `SINGLE_QUOTE` and `DOUBLE_QUOTE` tokens.
    * **Pipes/Logic:** Checks for operators (`PIPE`, `AND`, `OR`) that lack a necessary operand (e.g., `ls |` requires continuation).
    * **Parentheses:** `get_paren_balance` checks the overall balance of `PAREN_OPEN` and `PAREN_CLOSE` tokens.
3.  **Continuation Loop (`join_lines_until_balanced`):**
    * If `check_global_balance` returns `KEEP_TRYING` (unclosed quotes/pipes) or an unbalance count greater than zero (unclosed parentheses), the shell enters a loop.
    * The user is prompted with `>`.
    * The new line is concatenated with the previous lines, separated by a space (`ft_strjoin_multi`).
    * The process repeats until `BALANCE` is achieved or the user sends an EOF signal.
4.  **Syntax Error Handling:** If `check_global_balance` returns `CANT_CONTINUE` (e.g., a closing parenthesis without an opening one, or similar severe error), the process breaks, and the error is flagged, preventing the AST construction.

---

## 11.2. 💡 Command Correction: The Intelligent Safety Net

This feature is a sophisticated form of **proactive error handling** that prevents the frustrating "command not found" error by suggesting corrections for misspelled built-in commands. It is deliberately engineered for intelligence and minimal disruption.



https://github.com/user-attachments/assets/62c4d654-6cc8-4031-bf6f-f58b04326b14



### 11.2.1. The Advanced Correction Heuristic

The core of this feature is the function **`find_match`**, which employs an efficient heuristic to detect typographical errors with precision:

1.  **Length Tolerance:** The function strictly compares the length of the user's input against the list of known built-ins. A match is only considered if the length difference is **at most $\pm 1$ character**. This immediately eliminates distant or irrelevant suggestions.
2.  **Character Alignment:** It uses a modified character-by-character alignment to tolerate single common mistakes:
    * **Deletion:** (e.g., typing **`eho`** instead of **`echo`**).
    * **Insertion:** (e.g., typing **`echop`** instead of **`echo`**).
    * **Substitution/Transposition:** (e.g., typing **`exho`** instead of **`echo`**).
3.  **Annoyance Prevention (UX Focus):** The logic is tuned to avoid being "fastidiosa" (annoying):
    * Inputs consisting of a single space or containing symbols are ignored.
    * Single-character inputs are ignored unless they are highly ambiguous for a core command (e.g., typing `c` or `d` is still processed as a possible typo for `cd`).

### 11.2.2. Interactive Confirmation and In-Place Fix

The system is designed to be highly interactive via the **`ask_confirmation`** utility:

1.  **Suggestion Prompt:** When a probable typo is found, the user is immediately prompted with a clear question using specialized color coding: `"Did you mean %s? y/n"`.
2.  **Input Loop:** The process waits for the user to confirm (`y/yes`) or deny (`n/no`) the suggestion, looping until a valid response is given.
3.  **In-Place Correction:**
    * If the user confirms, the misspelled token's **`value`** string is **freed and replaced** with the correct built-in name.
    * The corrected token then continues through the pipeline, where subsequent passes of `transform_tokens_logic` automatically recognize the fixed built-in name and correctly flag the token type as **`BUILT_IN`**.
4.  **Error Handling on Denial:** If the user declines the suggestion, the `cmd_correction` function returns a `FAILURE` flag, forcing the shell to **clean up the prompt** and gracefully return to the main loop, preventing the command from proceeding to execution.

This interactive approach turns a potential hard failure into a soft, informative recovery, significantly enhancing the user's workflow.
---

## 11.3. 🎛️ UX and Session Management

### Personalized Welcome Message

The shell initializes with a customized banner and welcome sequence:

https://github.com/user-attachments/assets/a92f7c55-029f-42d5-8dba-1b3c7d41d668

* **Banner:** `print_minishell_title` displays an ASCII art banner using color-coded macros (T1-T5).
* **User Identity:** The `find_user` function attempts to retrieve the user's name from the **`USER`** environment variable. If unsuccessful, it falls back to prompting the user for their login.
* **Time-of-Day Greeting:** `print_time_of_day` determines the local time and displays a specialized greeting based on the hour (e.g., `Good morning`, `Burning the midnight oil?`).

### Session Timing and Cleanup

* **Start Time:** The session start time is recorded and printed upon initialization.
* **End Time:** `print_session_end` is called when the shell exits. It calculates the elapsed duration in minutes and seconds and prints a farewell message.

## 11.4. 🧩 Structural Execution Features

The shell fully implements the structural control flow operators required for complex scripting:

* **Subshells:** Encapsulated commands within parentheses (`(cmd1 | cmd2)`) are executed in a new, isolated process using `exec_subshell`, preserving I/O state and managing exit status propagation.
* **Sequencing:** Commands separated by **semicolons (`;`)** are executed unconditionally, one after the other, via `exec_semicolon`.
* **Background Execution:** Commands ending with the **background operator have their `background` flag set. These are executed by a child process, but the parent process immediately prints the PID and **does not wait** (`waitpid` is skipped), allowing the user to continue interacting with the shell immediately.

## GENERAL FLOW AND ARCHICTETURE

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
                   │(waits for user input)│
                   └─────────┬───────────┘
                 (SIGINT)    │   [EOF/exit]
                  PARENT      │   clean & exit
                  cleans     │
                  prompt     ▼
               ┌──────────────────────────────┐
               │ 1) TOKENIZER                 │
               │ - Divides the tokens         │
               │ - Identifies pipes, redirs   │
               │ - Manages quotes             │
               └───────────────┬──────────────┘
                               ▼
               ┌──────────────────────────────┐
               │ 2) EXPANSION                 │
               │ - Substitution $VAR, $?, tildes│
               │ - Expands wildcards          │
               │ - Respects quotes            │
               └───────────────┬──────────────┘
                               ▼
               ┌─────────────────────────────┐
               │ 3) AST BUILDER              │
               │ - Creates the AST           │
               │ - Gathers cmds/args         │
               │ - Orders pipes/redirs       │
               └──────────────┬──────────────┘
                              ▼
               ┌─────────────────────────────┐
               │ 4) EXECUTOR                 │
               │ - Iterates over AST         │
               │ - fork() for commands       │
               │ - Redirections              │
               │ - Pipes                     │
               └──────────────┬──────────────┘
                              │
                 ┌────────────┴─────────────┐
                 ▼                           ▼
      ┌─────────────────────┐     ┌─────────────────────┐
      │ PADRE               │     │ HIJO                │
      │──────────────────── │     │──────────────────── │
      │ - Waits with        │     │ - Restores Signals  │
      │   waitpid()         │     │   SIG_DFL           │
      │ - Manages Signals   │     │ - Executes CMDS     │
      │   (Ctrl+C)          │     │ - If signal → Dies  │
      └─────────────────────┘     └─────────────────────┘
                 │                           │
                 │       (exit/señal)        │
                 └───────────┬───────────────┘
                             ▼
                ┌─────────────────────────────┐
                │ clean_data(data)            │
                │ Goes back to the MAIN LOOP  │
                └─────────────────────────────┘

