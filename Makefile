# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 15:32:18 by hubourge          #+#    #+#              #
#    Updated: 2025/05/21 19:20:02 by hubourge         ###   ########.fr        #
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
				srcs/arp.c \
				srcs/print.c \
				
OBJ			= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

YELLOW		=\033[0;33m
BIBlue		=\033[1;94m
RED			=\033[1;31m
NC			=\033[0m

# /-------------- Makefile --------------/
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

# /-------------- Docker --------------/
DOCKER_COMPOSE = docker-compose.yml
HACKER_NAME = hacker
SOURCE_NAME = source
TARGET_NAME = target

docker: docker-build docker-stats

docker-build:
	@docker compose -f $(DOCKER_COMPOSE) up --build -d

docker-stop:
	@docker compose -f $(DOCKER_COMPOSE) stop

docker-down:
	@docker compose -f $(DOCKER_COMPOSE) down -v

docker-erase: docker-stop docker-down
	@docker container prune -f
	@docker network rm mitm_net || true
	@docker system prune -af --volumes

docker-stats:
	@ echo "\n$(BIBlue)======= Docker stats ======="
	@ docker ps
	@ echo ""
	@ docker volume ls
	@ echo ""
	@ docker network ls

	@ echo "\n$(BIBlue)======= Network info ======="
	@ echo "NAME     IP                MAC "
	@ echo "$(BIBlue)$(HACKER_NAME)   $(RED)$(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $(HACKER_NAME))\
		   $(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.MacAddress}}{{end}}' $(HACKER_NAME)) $(NC)"
	@ echo "$(BIBlue)$(SOURCE_NAME)   $(RED)$(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $(SOURCE_NAME))\
		   $(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.MacAddress}}{{end}}' $(SOURCE_NAME)) $(NC)"
	@ echo "$(BIBlue)$(TARGET_NAME)   $(RED)$(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $(TARGET_NAME))\
		   $(shell docker inspect -f '{{range .NetworkSettings.Networks}}{{.MacAddress}}{{end}}' $(TARGET_NAME)) $(NC)"
	@ echo "$(NC)"


.PHONY: all clean fclean re bonus docker-build docker-stop docker-down docker-erase docker-stats