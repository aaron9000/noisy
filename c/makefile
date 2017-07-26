# Helpers
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
objpath=$(patsubst %.o, obj/%.o, $(notdir $(patsubst %.c, %.o, $1)))

# Linux (default)
APP_EXE = bin/app.o
APP_ENTRY = app.c
TEST_EXE = bin/tests.o
TEST_ENTRY = test/tests.c
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Werror -Wshadow
LDFLAGS = -lGL -lGLU -lglut -lm
GAME_SOURCES := $(call rwildcard,game/,*.c)
VENDOR_SOURCES := $(call rwildcard,vendor/,*.c)
GAME_OBJECTS := $(call objpath, $(GAME_SOURCES))
VENDOR_OBJECTS := $(call objpath, $(VENDOR_SOURCES))

# OS X, OSTYPE not being declared
ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif
ifeq ($(OSTYPE),darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT  -Wno-deprecated
endif

# Recipes
app:

	@echo "*****************************************"
	@echo "*** cleaning binaries & object files  ***"
	@echo "*****************************************"
	find ./bin -name \*.o -delete
	find ./obj -name \*.o -delete
	find ./output -name \*.png -delete

	@echo "*****************************"
	@echo "*** building game objects ***"
	@echo "*****************************"
	for source in $(GAME_SOURCES) ; do \
		src=$$source; \
		srcfile=$$(basename "$$src"); \
		srcfilename="$${srcfile%.*}"; \
		obj=obj/$$srcfilename.o; \
		gcc -c -o $$obj $$src; \
	done

	@echo "*******************************"
	@echo "*** building vendor objects ***"
	@echo "*******************************"
	for source in $(VENDOR_SOURCES) ; do \
		src=$$source; \
		srcfile=$$(basename "$$src"); \
		srcfilename="$${srcfile%.*}"; \
		obj=obj/$$srcfilename.o; \
		gcc -c -o $$obj $$src; \
	done

	@echo "*******************************"
	@echo "*** building app entrypoint ***"
	@echo "*******************************"
	gcc -o $(APP_EXE) $(VENDOR_OBJECTS) $(GAME_OBJECTS) $(APP_ENTRY) $(CFLAGS) $(LDFLAGS)

	@echo "********************************"
	@echo "*** building test entrypoint ***"
	@echo "********************************"
	gcc -o $(TEST_EXE) $(VENDOR_OBJECTS) $(GAME_OBJECTS) $(TEST_ENTRY) $(CFLAGS) $(LDFLAGS)

	@echo "*********************"
	@echo "*** running tests ***"
	@echo "*********************"
	./$(TEST_EXE)
	ls -al bin/app.o
