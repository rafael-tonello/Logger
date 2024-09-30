#this makefile can be found in https://www.embarcados.com.br/introducao-ao-makefile/

# My third makefile
 
# Name of the project
DYNAMIC_LIB_NAME=logger.so
STATIC_LIB_NAME=liblogger.a

CUSTOM_INCLUDE_PATH_TMP1 := $(shell find ../../sources ! -path '*/.git/*' -type d)
CUSTOM_INCLUDE_PATH_TMP2 := $(shell find ./sources ! -path '*/.git/*' -type d) $(CUSTOM_INCLUDE_PATH_TMP1)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
CUSTOM_INCLUDE_PATH := $(addprefix -I,$(CUSTOM_INCLUDE_PATH_TMP2))



# .c files
C_SOURCE_TMP := $(shell find ../../sources  ! -path '*/.git/*' ! -path '*/tests/*' ! -path '*main.cpp' -name '*.cpp' -or -name '*.c' -or -name '*.s')
C_SOURCE := $(shell find ./sources  ! -path '*/.git/*' ! -path '*/tests/*' -name '*.cpp' -or -name '*.c' -or -name '*.s') $(C_SOURCE_TMP)

# .h files
H_SOURCE_TMP := $(shell find ../../sources  ! -path '*/.git/*' ! -path '*/tests/*' -name '*.hpp' -or -name '*.h' -or -name '*.inc')
H_SOURCE := $(shell find ./sources  ! -path '*/.git/*' ! -path '*/tests/*' -name '*.hpp' -or -name '*.h' -or -name '*.inc') $(H_SOURCE_TMP)


prebuild:
# 	prepares the folder built/gui. This folder contains files copied from GUI/resources. These files contains the HTML5 User interface.
	@ clear
	@ mkdir ./build | true
	@ cp -r ./sources/assets/* ./build | true
 
# Object files
temp:=$(subst ../../sources,./build/objects/main,$(C_SOURCE))
temp:=$(subst ./sources,./build/objects/lib,$(temp))

OBJ=$(subst .cpp,.o,$(temp))
 
# Compiler and linker
#CC=g++
CC=clang++


#		-pedantic
CC_FLAGS=-D__TESTING__\
        -ansi      \
		-c			\
		-pthread   \
		-std=c++20 \
		-fPIC	  \
		$(CUSTOM_INCLUDE_PATH)



#		-pedantic
LK_FLAGS=-D__TESTING__\
        -ansi      \
		-pthread   \
		-shared	   \
		-std=c++20 \
		$(CUSTOM_INCLUDE_PATH)

#AR_FLAGS=-t
AR_FLAGS=-rcs
#
# Compilation and linking
#
 

 
# Command used at clean target
RM = rm -rf
 
debug: CC=g++
debug: CC_FLAGS+=-g
debug: LK_FLAGS+=-g
debug: CC_FLAGS+=-ggdb
debug: LK_FLAGS+=-ggdb
debug: prebuild $(DYNAMIC_LIB_NAME)
	
all: CC_FLAGS+=-O3   
all: LK_FLAGS+=-O3
all: prebuild $(DYNAMIC_LIB_NAME)
 
$(DYNAMIC_LIB_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LK_FLAGS) -o build/$@
	ar $(AR_FLAGS) build/$(STATIC_LIB_NAME) $^
	ranlib build/$(STATIC_LIB_NAME)
	@ echo 'Finished building binary: $@'
	@ echo ' '

./build/objects/main/%.o: ../../sources/%.cpp ../../sources/%.h
	@ echo 'Building target using GCC compiler: $<'
	mkdir -p $(dir $@)
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

./build/objects/lib/%.o: ./sources/%.cpp ./sources/%.h
	@ echo 'Building target using GCC compiler: $<'
	mkdir -p $(dir $@)
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
 
./build/objects/lib/main.o: ./sources/main.cpp $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	mkdir -p $(dir $@)
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
	
	
clean:
	@ rm -rf ./build/objects
 
.PHONY: all clean