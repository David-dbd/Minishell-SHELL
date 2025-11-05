# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 17:55:34 by migarrid          #+#    #+#              #
#    Updated: 2025/11/05 00:50:27 by migarrid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                              Project Information                             #
# **************************************************************************** #
NAME				= minishell

# **************************************************************************** #
#                            Compiler and Flags                                #
# **************************************************************************** #
CC					= cc
#WFLAGS				= -Wall -Wextra -Werror
DFLAGS				= -g
#OFLAGS				= -Ofast -march=native -flto
SFLAGS				= -fsanitize=address,undefined
DEPFLAGS			= -MMD -MP

# **************************************************************************** #
#                               Shell Comands                                  #
# **************************************************************************** #
RM					= rm -rf
PRINT				= printf
MAKE				= make
MKDIR				= mkdir -p
NORM				= norminette
CMAKE				= cmake

# **************************************************************************** #
#                              Directories                                     #
# **************************************************************************** #
INC_DIR				= inc
LIB_DIR				= lib
OBJ_DIR				= obj
SRC_DIR				= src
LIBFT_DIR			= $(LIB_DIR)/libft_plus
ISOCLINE_DIR		= $(LIB_DIR)/isocline

# **************************************************************************** #
#                      File Paths and Dependencies                             #
# **************************************************************************** #
MAKEFILE			= Makefile
HEADER				= $(INC_DIR)/minishell.h \
					  $(INC_DIR)/minishell_macros.h \
					  $(INC_DIR)/minishell_structs.h
LIBFT_A				= $(LIBFT_DIR)/libft_plus.a
LIBFT_H				= $(LIBFT_DIR)/libft_plus.h
LIBFT_MAKEFILE		= $(LIBFT_DIR)/Makefile
ISOCLINE_A			= $(ISOCLINE_DIR)/build/release/libisocline.a
ISOCLINE_H			= $(ISOCLINE_DIR)/include/isocline.h
DEPS				= $(HEADER) $(MAKEFILE) $(LIBFT_H) $(LIBFT_MAKEFILE)
LDLIBS				= -lreadline -lhistory -ltermcap

# **************************************************************************** #
#                                   Colors                                     #
# **************************************************************************** #
DEFAULT				= \033[0;39m
GREEN 				= \033[0;92m
BLUE 				= \033[0;94m
MAGENTA 			= \033[0;95m
CYAN 				= \033[0;96m
YELLOW 				= \033[0;33m
RED 				= \033[0;31m
GREY 				= \033[38;5;240m
PURPLE 				= \033[38;5;141m
RESET 				= \033[0m
BOLD 				= \033[1m
CLEAR 				= \r\033[K

# **************************************************************************** #
#                               Source File                                    #
# **************************************************************************** #
SRCS =				main/main.c \
					main/utils/receive_input.c \
					main/utils/time/print_session_start.c \
					main/utils/time/print_session_end.c \
					main/utils/time/print_time_of_day.c \
					init/init_minishell.c \
					init/utils/cmd_structure/add_node_rule.c \
					init/utils/init/init_data.c \
					init/utils/init/init_env.c \
					init/utils/init/init_arg.c \
					init/utils/init/init_readline.c \
					init/utils/init/init_exec.c \
					init/utils/tokens/alloc_tokens.c \
					init/utils/tokens/add_token.c \
					init/utils/env/add_var.c \
					init/utils/env/make_envp.c \
					init/utils/env/update_shlvl.c \
					init/utils/env/update_var.c \
					init/utils/env/delete_var.c \
					init/utils/env/update_envp.c \
					init/utils/highlighther/highlighter.c \
					init/utils/highlighther/is_highlighter_I.c \
					init/utils/highlighther/is_highlighter_II.c \
					tokenizer/tokenizer.c \
					tokenizer/utils/is_tokens/is_and.c \
					tokenizer/utils/is_tokens/is_dolar.c \
					tokenizer/utils/is_tokens/is_heredoc.c \
					tokenizer/utils/is_tokens/is_or.c \
					tokenizer/utils/is_tokens/is_parenten.c \
					tokenizer/utils/is_tokens/is_pipe.c \
					tokenizer/utils/is_tokens/is_double_quote.c \
					tokenizer/utils/is_tokens/is_single_quote.c \
					tokenizer/utils/is_tokens/is_redir.c \
					tokenizer/utils/is_tokens/is_scape.c \
					tokenizer/utils/is_tokens/is_semicolon.c \
					tokenizer/utils/is_tokens/is_wildcar.c \
					tokenizer/utils/is_tokens/is_word.c \
					tokenizer/utils/is_tokens/is_not_token.c \
					tokenizer/utils/is_tokens/is_hash.c \
					tokenizer/utils/is_tokens/is_cmd.c \
					tokenizer/utils/is_tokens/is_cmdsubs.c \
					tokenizer/utils/is_tokens/is_type_I.c \
					tokenizer/utils/is_tokens/is_type_II.c \
					tokenizer/utils/is_tokens/is_type_III.c \
					tokenizer/utils/is_tokens/utils_is_double_quote.c \
					tokenizer/utils/check_syntax/is_it_quoted.c \
					tokenizer/utils/check_syntax/check_redir_input.c \
					tokenizer/utils/check_syntax/check_double_parent.c \
					tokenizer/utils/check_syntax/check_redir_output.c \
					tokenizer/utils/check_syntax/check_background.c \
					tokenizer/utils/check_syntax/check_semicolon.c \
					tokenizer/utils/check_syntax/check_open_parent.c \
					tokenizer/utils/check_syntax/check_closed_parent.c \
					tokenizer/utils/check_syntax/check_pipe.c \
					tokenizer/utils/check_syntax/check_or_and.c \
					tokenizer/utils/check_syntax/check_single_balance.c \
					tokenizer/utils/check_syntax/check_double_balance.c \
					tokenizer/utils/check_syntax/check_cmd_syntaxis.c \
					tokenizer/utils/check_syntax/check_parent_balance.c \
					tokenizer/utils/check_syntax/check_cmd_externs.c \
					tokenizer/utils/check_syntax/check_full_line_syntax.c \
					tokenizer/utils/simplify_tokens/simplify_tokens.c \
					tokenizer/utils/simplify_tokens/reorganize_tokens.c \
					tokenizer/utils/simplify_tokens/adjust_range_tokens.c \
					tokenizer/utils/trasnform_tokens/transform_tokens_logic.c \
					tokenizer/utils/trasnform_tokens/transform_cmd_to_word.c \
					tokenizer/utils/trasnform_tokens/transform_word_to_asignation.c \
					tokenizer/utils/trasnform_tokens/transform_invalid_asig_to_word.c \
					tokenizer/utils/trasnform_tokens/transform_asig_to_asig_plus.c \
					tokenizer/utils/trasnform_tokens/transform_asig_to_temp_asig.c \
					tokenizer/utils/trasnform_tokens/transform_word_to_file.c \
					tokenizer/utils/trasnform_tokens/transform_command_builtin_lowercase.c \
					tokenizer/utils/trasnform_tokens/transform_cmd_to_built_in.c \
					tokenizer/utils/cmd_sugestion/cmd_correction.c \
					expansion/expansion.c \
					expansion/send_tokens_for_asig.c \
					expansion/utils/find_swap/copy_key.c \
					expansion/utils/find_swap/copy_value.c \
					expansion/utils/find_swap/extract_key.c \
					expansion/utils/find_swap/find_key_in_list.c \
					expansion/utils/find_swap/expand_empty_str.c \
					expansion/utils/asignation/asignation.c \
					expansion/utils/asignation/verify_if_already_set.c \
					expansion/utils/asignation/is_it_asig.c \
					expansion/utils/asignation/check_externs_syntax.c \
					expansion/utils/asignation/check_asignation_syntax.c \
					ast/ast_builder.c \
					ast/parser/parse_sequence.c \
					ast/parser/parse_and_or.c \
					ast/parser/parse_pipes.c \
					ast/parser/parse_subshell.c \
					ast/parser/parse_assignations.c \
					ast/parser/parse_cmd.c \
					ast/utils/create_node.c \
					ast/utils/get_args_for_binary.c \
					ast/utils/get_redirs.c \
					ast/utils/get_arg_types.c \
					ast/utils/get_temp_asignations.c \
					ast/utils/get_background.c \
					ast/utils/safe_index_plus.c \
					ast/utils/get_heredoc.c \
					ast/utils/expand_alias.c \
					executor/executor.c \
					executor/exec/exec_semicolon.c \
					executor/exec/exec_and.c \
					executor/exec/exec_or.c \
					executor/exec/exec_pipe.c \
					executor/exec/exec_command.c \
					executor/exec/exec_subshell.c \
					executor/exec/exec_builtin.c \
					executor/utils/which_builtin.c \
					executor/utils/get_path_cmd.c \
					executor/utils/eliminate_token.c \
					executor/utils/eliminate_temp_asig.c \
					executor/utils/apply_properties.c \
					executor/utils/apply_redirs.c \
					executor/utils/apply_temp_asig.c \
					signals/setup_signals.c \
					signals/handler_signals.c \
					signals/check_signals.c \
					builtin/my_env.c \
					builtin/my_echo.c \
					builtin/my_export.c \
					builtin/my_unset.c \
					builtin/my_pwd.c \
					builtin/my_cd.c \
					builtin/my_exit.c \
					builtin/my_unalias.c \
					builtin/my_alias/my_alias_part_1.c \
					builtin/my_alias/my_alias_part_2.c \
					debug/print_ast_tree.c \
					debug/print_tokens_debug.c \
					exit/exit.c \
					exit/error.c \
					exit/clean_I.c \
					exit/clean_II.c \
					exit/clean_III.c

# **************************************************************************** #
#                              Progress Bars                                   #
# **************************************************************************** #
SRC_COUNT_TOT := $(shell echo -n $(SRCS) | wc -w)
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell echo -n $(SRCS) | wc -w)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))

# **************************************************************************** #
#                               Object File                                    #
# **************************************************************************** #
# Create directories
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)
	@$(MKDIR) $(DEPS_DIR)

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS_DIR	= $(OBJ_DIR)
DEPS_FILES	= $(SRCS:%.c=$(DEP_DIR)/%.d)

# Rule to compile archive .c to ,o with progress bars
${OBJ_DIR}/%.o: ${SRC_DIR}/%.c $(DEPS) $(LIBFT_A) | $(OBJ_DIR)
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINT) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)...\n" "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(MKDIR) $(dir $@)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) -I$(INC_DIR) $(DEPFLAGS) -c -o $@ $<

# Include .deps files
-include $(DEPS_FILES)

# **************************************************************************** #
#                              Targets                                         #
# **************************************************************************** #

all: $(ISOCLINE_A) $(LIBFT_A) $(NAME)

# Build executable
$(NAME): $(OBJS) $(LIBFT_A) $(HISTORY_A) $(ISOCLINE_A)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) $(OBJS) $(LIBFT_A) $(ISOCLINE_A) -I$(INC_DIR) $(LDLIBS) -o $(NAME)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: ${RED}${BOLD}${NAME} ${RESET}compiled ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild libft.a
$(LIBFT_A): FORCE $(LIBFT_MAKEFILE) $(LIBFT_H)
	@$(MAKE) -s -C $(LIBFT_DIR)

# Rebuild isocline library
$(ISOCLINE_A):
	@$(PRINT) "Compiling $(BLUE)isocline library$(DEFAULT)...\n"
	@$(MKDIR) $(ISOCLINE_DIR)/build/release
	@cd $(ISOCLINE_DIR)/build/release && $(CMAKE) ../.. > /dev/null 2>&1 && $(CMAKE) --build . > /dev/null 2>&1

# Test minishell rapidly
test:
	@clear
	@$(MAKE) all
	@./minishell

# Test leaks in minishell
leaks:
	@clear
	@$(MAKE) all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
#	@valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible --track-origins=yes ./minishell

# Test the norminette in my .c files
norm:
	-@$(NORM) $(LIBFT_DIR)
	-@$(NORM) $(INC_DIR)
	-@$(NORM) $(SRC_DIR)

# Clean object files
clean:
	@$(MAKE) clean -s -C $(LIBFT_DIR)
	@$(RM) $(ISOCLINE_DIR)/build
	@$(RM) $(OBJ_DIR)
	@$(PRINT) "${CLEAR}${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Objects were cleaned ${GREEN}successfully${RESET}.\n${RESET}"

# Full clean
fclean: clean
	@$(MAKE) fclean -s -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Project cleaned ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild everything
re: fclean all

# Force verification every build
FORCE:

# Phony targets
.PHONY: all clean fclean re
.DEFAULT_GOAL := all
