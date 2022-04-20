##
## EPITECH PROJECT, 2020
## $MAKEFILE
## File description:
## makefile
##

CXX	=	g++

COPY	=	cp -f
COPY_FILE	=	$(COPY)
COPY_DIR	=	$(COPY) -r

DELETE	=	rm -f
DELETE_FILE	=	$(DELETE)
DELETE_DIR	=	$(DELETE) -r

SOURCE	=	CoreFrequencyDomainFeatures.cpp		\
			CoreTimeDomainFeatures.cpp			\
			Gist.cpp							\
			MFCC.cpp							\
			OnsetDetectionFunction.cpp			\
			WindowFunctions.cpp					\
			Yin.cpp								\

OBJ	=	$(SOURCE:.cpp=.o)

CXXFLAGS	+=	-W -Wall -Wextra
CXXFLAGS	+=	$(INCLUDE_DIRS)
CXXFLAGS	+=	-std=c++2a
CXXFLAGS	+=	-lm -lfftw3

NAME	=	libGist.a

all:	$(NAME)

$(NAME):	$(OBJ)
	@ar rc $(NAME) $(OBJ)
	@$(DELETE_FILE) $(OBJ)

clean:
	@$(DELETE_FILE) $(OBJ)

fclean:	clean
	@$(DELETE_FILE) $(NAME)

re:	fclean all

.PHONY: all $(NAME) clean fclean re
