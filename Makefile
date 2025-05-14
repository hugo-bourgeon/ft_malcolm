# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 15:32:18 by hubourge          #+#    #+#              #
#    Updated: 2025/05/14 18:15:22 by hubourge         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			= gcc

NAME		= ft_malcolm

CFLAGS		=  -Wall -Wextra -Werror -g

INCLUDE		= -I includes

LIBFT_DIR	= ./libft
LIBFT		= $(LIBFT_DIR)/libft.a
LIB			= -lm

OBJ_DIR		= obj

SRC			=	srcs/main.c \
				srcs/init.c \
				srcs/utils.c \
				srcs/parsing.c \
				srcs/process.c \
				
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

YELLOW		=\033[0;33m
BIBlue		=\033[1;94m
NC			=\033[0m

all: $(LIBFT) $(OBJ) 
	@ echo "$(YELLOW)Compiling ft_malcolm...$(NC)"
	@ $(CXX) $(CFLAGS) ${INCLUDE} -o $(NAME) $(OBJ) -L $(LIBFT_DIR) -lft $(LIB)
	@ echo "$(BIBlue)"
	@ echo "   __ _                       _           _            "
	@ echo "  / _| |_     _ __ ___   __ _| | ___ ___ | |_ __ ___   "
	@ echo " | |_| __|   | '_ \` _ \ / _\` | |/ __/ _ \| | '_ \` _ \  "
	@ echo " |  _| |_    | | | | | | (_| | | (_| (_) | | | | | | | "
	@ echo " |_|  \__|___|_| |_| |_|\__,_|_|\___\___/|_|_| |_| |_| "
	@ echo "        |_____|                                        "
	@ echo "                                          by hubourge  "
	@ echo "$(NC)"

bonus: all

$(LIBFT):
	@ echo "$(YELLOW)Compiling libft...$(NC)"	
	@ make -sC $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@ mkdir -p $(@D)
	@ $(CXX) $(CFLAGS) ${INCLUDE} -c $< -o $@

clean:
	@ echo "$(YELLOW)Cleaning in progress...$(NC)"
	@ rm -rf $(OBJ) obj
	@ make -sC $(LIBFT_DIR) clean
	@ echo "$(YELLOW)Cleaning completed$(NC)\n"

fclean: clean
	@ rm -rf $(NAME)
	@ make -sC $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus