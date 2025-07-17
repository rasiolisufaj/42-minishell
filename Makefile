# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 15:30:49 by raisufaj          #+#    #+#              #
#    Updated: 2025/07/17 11:19:45 by raisufaj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler & Flags
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -g

# Project Name
NAME    := minishell
NAME_BONUS := minishell_bonus

# Directories
INCDIR  := includes
LIBDIR  := libft
SRCDIR  := srcs
BUILDDIR := build

# Libft
LIBFT   := $(LIBDIR)/libft.a
LIBFT_INC := -I$(LIBDIR)

# Includes
INCLUDES := -I$(INCDIR) $(LIBFT_INC)

# Libraries
LIBS := -lreadline -lhistory

# Source and object files
# SRC_FILES := $(shell find $(SRCDIR) -name "*.c")

SRC_FILES := \
	srcs/builtins/add_exported.c srcs/builtins/cd_helpers.c \
	srcs/builtins/cd.c srcs/builtins/echo.c \
	srcs/builtins/env.c srcs/builtins/export.c \
	srcs/builtins/exit.c srcs/builtins/export_utils.c \
	srcs/builtins/pwd.c srcs/builtins/unset.c \
	srcs/executor/env_to_envp.c srcs/executor/exec_builtins.c \
	srcs/executor/exec_cmd.c srcs/executor/exec_pipe.c \
	srcs/executor/executor.c srcs/executor/external_cmd.c \
	srcs/executor/find_path.c srcs/executor/heredoc.c \
	srcs/executor/init_and_dup.c srcs/executor/mult_heredoc.c \
	srcs/executor/redirections.c \
	srcs/expands/consume_double_quote.c srcs/expands/consume_single_quote.c \
	srcs/expands/consume_unquoted.c srcs/expands/expand_helpers.c \
	srcs/expands/expand_token_helpers.c srcs/expands/expand_token.c \
	srcs/expands/expand_variable.c srcs/expands/expand.c \
	srcs/expands/word_split.c \
	srcs/lexer/lexer.c srcs/lexer/meta_tokens.c \
	srcs/lexer/quote_token.c srcs/lexer/tokenizer.c \
	srcs/lexer/word_token.c \
	srcs/main/init_shell.c srcs/main/prompt.c \
	srcs/main/main.c \
	srcs/parser/parse_and_or.c srcs/parser/parse_cmd.c \
	srcs/parser/parse_group.c srcs/parser/parse_pipe.c \
	srcs/parser/parser.c \
	srcs/utils/all_utilities.c srcs/utils/parser_utils.c \
	srcs/utils/signals.c srcs/utils/utils_2.c \
	srcs/utils/sig_heredoc.c \
	srcs/wildcards/wildcards_directory.c srcs/wildcards/wildcards_handlers.c \
	srcs/wildcards/wildcards_pattern_match.c srcs/wildcards/wildcards_token_utils.c \
	srcs/wildcards/wildcards.c

OBJ_FILES := $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC_FILES))

# Colors
GREEN := \033[0;32m
NC := \033[0m

# Default target
all: $(LIBFT) $(NAME)

# Build libft
$(LIBFT):
	@$(MAKE) -C $(LIBDIR)

# Rule to build the final binary
$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) $(LIBS) $(INCLUDES)
	@echo "$(GREEN)[OK] minishell built!$(NC)"

bonus: $(LIBFT) $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) $(LIBS) $(INCLUDES)
	@echo "$(GREEN)[OK] minishell built!$(NC)"

# Pattern rule for compiling .c to .o in build/
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Cleaning rules
clean:
	@$(MAKE) -C $(LIBDIR) clean
	@rm -rf $(BUILDDIR)
	@echo "Cleaned object files."

fclean: clean
	@$(MAKE) -C $(LIBDIR) fclean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "Removed $(NAME)."

re: fclean all
re_bonus: fclean bonus

.PHONY: all clean fclean re re_bonus bonus
