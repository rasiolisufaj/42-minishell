# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/16 13:04:07 by raisufaj          #+#    #+#              #
#    Updated: 2025/05/14 14:21:03 by raisufaj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RESET = \033[0m

SOURCES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
		ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
		ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
		ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
		ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
		ft_printf.c print_char.c print_utils.c ft_lstadd_back_bonus.c \
		ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c \
		ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
		ft_lstnew_bonus.c ft_lstsize_bonus.c get_next_line_utils.c \
		get_next_line.c

OBJECTS = $(SOURCES:.c=.o)

%.o : %.c
		@echo "$(YELLOW)[Compiling] $<$(RESET)"
		@$(CC) $(CFLAGS) -c $< -o $@ -I ./

$(NAME) : $(OBJECTS)
		@ar rc $(NAME) $(OBJECTS)
		@echo "$(GREEN)[Archive Successfully Created] $(NAME)$(RESET)"

all : $(NAME)

clean :
		@$(RM) $(OBJECTS)
		@echo "$(BLUE)[Cleaned object files]$(RESET)"

fclean : clean
		@$(RM) $(NAME)
		@echo "$(BLUE)[Removed] $(NAME)$(RESET)"

re : fclean all

.PHONY : all clean fclean re bonus