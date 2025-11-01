# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/28 19:51:37 by migarrid          #+#    #+#              #
#    Updated: 2025/10/31 16:44:01 by davdiaz-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nombre de la biblioteca
NAME = libft_plus.a

# Compilador y opciones
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Colores
DEFAULT = \033[0;39m
GREEN = \033[0;92m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m

# Comandos
PRINTF = printf
RM = rm -f

# Archivos fuente obligatorios
SRCS =	libft/ft_isalpha.c \
		libft/ft_isdigit.c \
		libft/ft_isalnum.c \
		libft/ft_isascii.c \
		libft/ft_isprint.c \
		libft/ft_issign.c \
		libft/ft_isspace.c \
		libft/ft_isspecial.c \
		libft/ft_ishex.c \
		libft/ft_strlen.c \
		libft/ft_count_char.c \
		libft/ft_memset.c \
		libft/ft_bzero.c \
		libft/ft_memcpy.c \
		libft/ft_memmove.c \
		libft/ft_strlcpy.c \
		libft/ft_strlcat.c \
		libft/ft_toupper.c \
		libft/ft_tolower.c \
		libft/ft_strchr.c \
		libft/ft_strrchr.c \
		libft/ft_strcmp.c \
		libft/ft_strncmp.c \
		libft/ft_strmatch_cmp.c \
		libft/ft_memchr.c \
		libft/ft_memcmp.c \
		libft/ft_strnstr.c \
		libft/ft_atoi.c \
		libft/ft_atol.c \
		libft/ft_atoi_range.c \
		libft/ft_strtol.c \
		libft/ft_strtof.c \
		libft/ft_square.c \
		libft/ft_max.c \
		libft/ft_calloc.c \
		libft/ft_realloc.c \
		libft/ft_strdup.c \
		libft/ft_strndup.c \
		libft/ft_substr.c \
		libft/ft_strjoin.c \
		libft/ft_strjoin_multi.c \
		libft/ft_strtrim.c \
		libft/ft_strfilter_c.c\
		libft/ft_split.c \
		libft/ft_itoa.c \
		libft/ft_strmapi.c \
		libft/ft_striteri.c \
		libft/ft_putchar_fd.c \
		libft/ft_putstr_fd.c \
		libft/ft_putendl_fd.c \
		libft/ft_putnbr_fd.c \
		libft/ft_free_str_array.c \
		libft/ft_free_int_matrix.c \
		libft/ft_lstnew_bonus.c \
		libft/ft_lstadd_front_bonus.c \
		libft/ft_lstsize_bonus.c \
		libft/ft_lstlast_bonus.c \
		libft/ft_lstadd_back_bonus.c \
		libft/ft_lstdelone_bonus.c \
		libft/ft_lstclear_bonus.c \
		libft/ft_lstiter_bonus.c \
		libft/ft_lstmap_bonus.c \
		ft_printf_fd/ft_printf_fd_bonus.c \
		ft_printf_fd/ft_var_printf_fd_bonus.c \
		ft_printf_fd/ft_apply_flags_bonus.c \
		ft_printf_fd/ft_apply_hash_bonus.c \
		ft_printf_fd/ft_apply_left_align_bonus.c \
		ft_printf_fd/ft_apply_plus_bonus.c \
		ft_printf_fd/ft_apply_precision_bonus.c \
		ft_printf_fd/ft_apply_space_bonus.c \
		ft_printf_fd/ft_apply_zero_pad_bonus.c \
		ft_printf_fd/ft_apply_min_width_bonus.c \
		ft_printf_fd/ft_print_char_fd_bonus.c \
		ft_printf_fd/ft_print_str_fd_bonus.c \
		ft_printf_fd/ft_print_ptr_fd_bonus.c \
		ft_printf_fd/ft_print_num_fd_bonus.c \
		ft_printf_fd/ft_print_unsigned_fd_bonus.c \
		ft_printf_fd/ft_print_hex_fd_bonus.c \
		ft_printf_fd/ft_print_float_fd_bonus.c \
		ft_printf_fd/ft_print_float_fd_utils_bonus.c \
		ft_printf_fd/ft_itoa_base_bonus.c \
		ft_printf_fd/ft_utoa_bonus.c \
		get_next_line/get_next_line_bonus.c \

# Objetos obligatorios
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# Variables de progreso
SRC_COUNT_TOT := $(shell echo -n $(SRCS) | wc -w)
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell echo -n $(SRCS) | wc -w)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))

# Regla principal: compilar la biblioteca
all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

# Regla para compilar archivos .c a .o con barra de progreso
%.o: %.c libft_plus.h Makefile
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(CC) $(CFLAGS) -I. -MMD -MP -c -o $@ $<

# Incluir archivos de dependencia
-include $(DEPS)

# Limpiar objetos
clean:
	@$(RM) $(OBJS) $(DEPS)

# Limpiar todo (objetos + biblioteca)
fclean: clean
	@$(RM) $(NAME)
	@$(RM) ../$(NAME)

# Recompilar todo desde cero sin bonus
re: fclean all

# Asegurar que las reglas se ejecuten como comandos del make
.PHONY: all clean fclean re bonus rebonus
