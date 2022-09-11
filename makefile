#this makefile can be found in https://www.embarcados.com.br/introducao-ao-makefile/

# My third makefile
 
# Name of the project
PROJ_NAME=CapitalManager
CUSTOM_INCLUDE_PATH = -I"./sources"

# .c files
C_SOURCE=$(wildcard ./sources/*.cpp)
C_SOURCE+=$(wildcard ./sources/common/misc/*.cpp)
 
# .h files
H_SOURCE=$(wildcard ./sources/*.h)
H_SOURCE+=$(wildcard ./sources/common/misc/*.h)

objFolder:
	@ mkdir -p objects/common/misc

prebuild:
# 	prepares the folder built/gui. This folder contains files copied from GUI/resources. These files contains the HTML5 User interface.
	@ clear
	@ mkdir ./build | true
	@ cp -r ./sources/copyToBuildFolder/* ./build | true
 
# Object files
OBJ=$(subst .cpp,.o,$(subst ./sources,./objects,$(C_SOURCE)))
 
# Compiler and linker
CC= g++
 
# Flags for compiler
CC_FLAGS=-c			\
		 -pthread   \
		 -lssl		\
		 -lcrypto	\
		 -ggdb3 \
		 -std=c++20 \
		 $(CUSTOM_INCLUDE_PATH)

# Flags for linker
LK_FLAGS=-pthread   \
		 -lssl		\
		 -lcrypto	\
		 -ggdb3 \
		 -std=c++20 \
		 $(CUSTOM_INCLUDE_PATH)
 
# Command used at clean target
RM = rm -rf
 
#
# Compilation and linking
#
all: objFolder prebuild $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LK_FLAGS) -o build/$@
	@ echo 'Finished building binary: $@'
	@ echo ' '
 
./objects/%.o: ./sources/%.cpp ./sources/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
 
./objects/main.o: ./sources/main.cpp $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
	
clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rm -rf objects
 
.PHONY: all clean
