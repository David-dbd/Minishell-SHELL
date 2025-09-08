# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 17:55:34 by migarrid          #+#    #+#              #
#    Updated: 2025/09/08 19:08:25 by migarrid         ###   ########.fr        #
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
#OFLAGS				= -O2 -march=native -flto
SLAGS				= -fsanitize=address

# **************************************************************************** #
#                               Shell Comands                                  #
# **************************************************************************** #
RM					= rm -rf
PRINT				= printf
MAKE				= make
MKDIR				= mkdir -p
NORM				= norminette

# **************************************************************************** #
#                              Directories                                     #
# **************************************************************************** #
INC_DIR				= inc
LIB_DIR				= lib
OBJ_DIR				= obj
SRC_DIR				= src
LIBFT_DIR			= $(LIB_DIR)/libft_plus
READLINE_DIR		= $(LIB_DIR)/readline

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
HISTORY_A			= $(READLINE_DIR)/libhistory.a
READLINE_A			= $(READLINE_DIR)/libreadline.a
READLINE_HEADERS	=	ansi_stdlib.h chardefs.h colors.h config.h histlib.h \
						history.h keymaps.h parse-colors.h posixdir.h posixjmp.h \
						posixselect.h posixstat.h readline.h rlconf.h rldefs.h \
						rlmbutil.h rlprivate.h rlshell.h rlstdc.h rltty.h \
						rltypedefs.h rlwinsize.h tcap.h tilde.h xmalloc.h
READLINE_H			= $(addprefix $(READLINE_DIR)/, $(READLINE_HEADERS))
READLINE_MAKEFILE	= $(READLINE_DIR)/Makefile
READLINE_CONFIGURE	= $(READLINE_DIR)/configure
LDLIBS				= $(READLINE_A) $(HISTORY_A) -ltermcap
DEPS				= $(HEADER) $(MAKEFILE) $(LIBFT_H) $(LIBFT_MAKEFILE)

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
					main/utils/recieve_input.c \
					main/utils/time/print_session_start.c \
					main/utils/time/print_session_end.c \
					main/utils/time/print_time_of_day.c \
					init/init_data.c \
					init/init_env.c \
					init/utils/alloc_tokens.c \
					init/utils/add_token.c \
					init/utils/add_var.c \
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
					tokenizer/utils/check_syntax/logic_trans_args_cmd.c \
					tokenizer/utils/check_syntax/check_redir_input.c \
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
					expansion/expansion.c \
					expansion/utils/simplify_tokens/simplify_tokens.c \
					expansion/utils/simplify_tokens/reorganize_tokens.c \
					expansion/utils/simplify_tokens/adjust_range_tokens.c \
					ast/ast_builder.c \
					executor/executor.c \
					signals/init_signals.c \
					signals/signal_handler.c \
					builtin/env.c \
					exit/exit.c \
					exit/error.c \
					exit/clean.c

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

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS_DIR	= $(OBJ_DIR)
DEPS_FILES	= $(SRCS:%.c=$(DEP_DIR)/%.d)

# Rule to compile archive .c to ,o with progress bars
${OBJ_DIR}/%.o: ${SRC_DIR}/%.c $(DEPS) $(LIBFT_A) | $(OBJ_DIR)
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINT) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)...\n" "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(MKDIR) $(dir $@)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) -I$(INC_DIR) -MMD -MP -c -o $@ $<

# Include .deps files
-include $(DEPS_FILES)

# **************************************************************************** #
#                              Targets                                         #
# **************************************************************************** #

all: $(READLINE_A) $(LIBFT_A) $(NAME)

# Build executable
$(NAME): $(OBJS) $(LIBFT_A) $(READLINE_A) $(HISTORY_A)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) $(RFLAGS) $(OBJS) $(LIBFT_A) -I$(INC_DIR) $(LDLIBS) -o $(NAME)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: ${RED}${BOLD}${NAME} ${RESET}compiled ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild libft.a
$(LIBFT_A): FORCE $(LIBFT_MAKEFILE) $(LIBFT_H)
	@$(MAKE) -s -C $(LIBFT_DIR)

# Rebuild readline libraries
$(READLINE_A): $(READLINE_DIR)/config.h
	@$(PRINT) "Compiling $(BLUE)readline libraries$(DEFAULT)...\n"
	@$(MAKE) -s -C $(READLINE_DIR)

$(READLINE_DIR)/config.h:
	@$(PRINT) "Configuring $(BLUE)readline$(DEFAULT)...\n"
	@cd $(READLINE_DIR) && ./configure --enable-static --disable-shared > /dev/null 2>&1

# Test the norminette in my .c files
norm:
	-@$(NORM) $(LIBFT_DIR)
	-@$(NORM) $(INC_DIR)
	-@$(NORM) $(SRC_DIR)

# Clean object files
clean:
	@$(MAKE) clean -s -C $(LIBFT_DIR)
	@$(MAKE) clean -s -C $(READLINE_DIR)
	@$(RM) $(OBJ_DIR)
	@$(PRINT) "${CLEAR}${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Objects were cleaned ${GREEN}successfully${RESET}.\n${RESET}"

# Full clean
fclean: clean
	@$(MAKE) fclean -s -C $(LIBFT_DIR)
	@$(MAKE) distclean -s -C $(READLINE_DIR)
	@$(RM) $(NAME)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Project cleaned ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild everything
re: fclean all

# Force verification every build
FORCE:

# Phony targets
.PHONY: all clean fclean re
.DEFAULT_GOAL := all
