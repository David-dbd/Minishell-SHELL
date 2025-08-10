# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 17:55:34 by migarrid          #+#    #+#              #
#    Updated: 2025/08/11 00:01:56 by migarrid         ###   ########.fr        #
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
OFLAGS				= -O2 -march=native -flto
SFLAGS				= -fsanitize=address

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
OBJ_BONUS_DIR		= $(OBJ_DIR)/bonus
SRC_DIR				= src
SRC_BONUS_DIR 		= $(SRC_DIR)/bonus
LIBFT_DIR			= $(LIB_DIR)
DEPS				= $(HEADER) $(MAKEFILE) $(LIBFT_H) $(LIBFT_MAKEFILE)

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
LDLIBS				= -lreadline

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
					signals/init_signals.c \
					signals/signal_handler.c \
					exit/exit.c \
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

BONUS_COUNT_TOT := $(shell echo -n $(SRC_BONUS) | wc -w)
ifeq ($(shell test $(BONUS_COUNT_TOT) -le 0; echo $$?),0)
	BONUS_COUNT_TOT := $(shell echo -n $(SRC_BONUS) | wc -w)
endif
BONUS_COUNT := 0
BONUS_PCT = $(shell expr 100 \* $(BONUS_COUNT) / $(BONUS_COUNT_TOT))

# **************************************************************************** #
#                               Object File                                    #
# **************************************************************************** #
# Create directories
$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)
$(OBJ_BONUS_DIR):
	@$(MKDIR) $(OBJ_BONUS_DIR)

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS 	= $(SRC_BONUS:%.c=$(OBJ_BONUS_DIR)/%.o)

# Rule to compile archive .c to ,o with progress bars
${OBJ_DIR}/%.o: ${SRC_DIR}/%.c $(DEPS) | $(OBJ_DIR)
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINT) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)...\n" "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(MKDIR) $(dir $@)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) -I. -c -o $@ $<

# Rule to compile archive .c to ,o with progress bars (Bonus)
$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c $(DEPS) | $(OBJ_BONUS_DIR)
	@$(eval BONUS_COUNT = $(shell expr $(BONUS_COUNT) + 1))
	@$(PRINT) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(MAGENTA)$<$(DEFAULT)...\n" "" $(BONUS_COUNT) $(BONUS_COUNT_TOT) $(BONUS_PCT)
	@$(MKDIR) $(dir $@)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) -I. -c -o $@ $<

# **************************************************************************** #
#                              Targets                                         #
# **************************************************************************** #

all: $(LIBFT_A) $(NAME)

# Build executable
$(NAME): $(OBJS)
	@$(CC) $(WFLAGS) $(DFLAGS) $(SFLAGS) $(OFLAGS) $(RFLAGS) $(OBJS) $(LIBFT_A) -I$(INC_DIR) $(LDLIBS) -o $(NAME)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: ${RED}${BOLD}${NAME} ${RESET}compiled ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild libft.a
$(LIBFT_A): $(LIBFT_MAKEFILE) $(LIBFT_H)
	@$(MAKE) -s -C $(LIBFT_DIR)

# Rule to compile bonus
Bonus: all

# Test the norminette in my .c files
norm:
	@$(NORM) $(SRC_DIR)
	@$(NORM) $(INC_DIR)
	@$(NORM) $(LIBFT_DIR)

# Clean object files
clean:
	@$(MAKE) clean -s -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR)
	@$(PRINT) "${CLEAR}${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Objects were cleaned ${GREEN}successfully${RESET}.\n${RESET}"

# Full clean
fclean: clean
	@$(MAKE) fclean -s -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@$(RM) $(NAME_BONUS)
	@$(PRINT) "${CLEAR}${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}${GREEN}»${RESET} [${PURPLE}${BOLD}${NAME}${RESET}]: Project cleaned ${GREEN}successfully${RESET}.${GREY}\n${RESET}${GREY}────────────────────────────────────────────────────────────────────────────\n${RESET}"

# Rebuild everything
re: fclean all

# Rebuild everything including bonus
rebonus: fclean all bonus

# Phony targets
.PHONY: all bonus clean fclean re rebonus
.DEFAULT_GOAL := all
