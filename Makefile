# **************************************************************************** #
#                                  MINISHELL                                   #
# **************************************************************************** #

# Compiler & Flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g

# Project Name
NAME    := minishell

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
SRC_FILES := main.c $(shell find $(SRCDIR) -name "*.c")
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
	@rm -f $(NAME)
	@echo "Removed $(NAME)."

re: fclean all

.PHONY: all clean fclean re
