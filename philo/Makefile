# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/22 17:49:50 by wnid-hsa          #+#    #+#              #
#    Updated: 2025/08/23 01:19:01 by wnid-hsa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread

SRCS = cleaning.c cust_mtx_func.c forks.c gc_malloc.c init.c main.c \
       parssing.c utils.c \

OBJS = $(SRCS:.c=.o)
HDRS = philosphers.h
NAME = philo

all: $(NAME)

%.o:%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o philo



clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY : clean