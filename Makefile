CFLAGS := -Wall -Wextra -g
LDFLAGS := -lgsl -lgslcblas -lm

SRC = main.c \
	gnuplot_affichage.c \
	rkf45.c \
	potentiel.c \
	powell.c \

OBJ = $(SRC:%.c=obj/%.o)

DEPS = $(OBJ:%.o=%.d)

all : obj/ exe resultats/

resultats/ :
	@mkdir -p resultats

obj/ :
	@mkdir -p obj

exe : $(OBJ)
	@gcc $^ -o $@ $(LDFLAGS)
	@echo "Compilation de l'éxecutable terminée."

obj/%.o : src/%.c
	@echo "Compilation de $< ..."
	@gcc -MMD -c $< -o $@ $(CFLAGS)
# MMD

clean :
	@rm -rf obj
	@rm -f exe
	@echo "Tout est effacé"

-include $(DEPS)
