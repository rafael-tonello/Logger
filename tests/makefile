 # My third makefile
 
# Name of the project
PROJ_NAME=tests
# .c files

#teste app includes
CUSTOM_INCLUDE_PATH = -I"./sources"

#main app includes
CUSTOM_INCLUDE_PATH += -I"../sources"
CUSTOM_INCLUDE_PATH += -I"../sources/common/libs/DynamicVar/sources"
CUSTOM_INCLUDE_PATH += -I"../sources/common/libs/libs.json_maker/sources"



#test app files
C_SOURCE=$(wildcard ./sources/*.cpp)

#main app files
C_SOURCE+=$(wildcard ../sources/*.cpp)
C_SOURCE+=$(wildcard ../sources/common/libs/DynamicVar/sources/*.cpp)
C_SOURCE+=$(wildcard ../sources/common/libs/libs.json_maker/sources/*.cpp)
  
# .h files
#test app files
H_SOURCE=$(wildcard ./sources/*.h)

#main app files
H_SOURCE+=$(wildcard ../sources//*.h)
H_SOURCE+=$(wildcard ../sources/Common/libs/DynamicVar/sources/*.h)
H_SOURCE+=$(wildcard ../sources/Common/libs/libs.json_maker/sources/*.h)

objFolder:
	@ mkdir -p objects/tests/

#	the main app directories
	@ mkdir -p objects/main/Common/libs/DynamicVar/sources
	@ mkdir -p objects/main/Common/libs/libs.json_maker/sources
	

prebuild:
# 	prepares the folder built/gui. This folder contains files copied from GUI/resources. These files contains the HTML5 User interface.
	@ clear
	@ mkdir ./build | true
	@ cp -r ./sources/copyToBuildFolder/* ./build | true
 
# Object files
OBJ=$(subst .cpp,.o,$(subst ./sources,./objects/tests,$(subst ../sources,./objects/main,$(C_SOURCE))))
 
# Compiler and linker
CC=g++
 
# Flags for compiler
#		 -W         
#        -Wall      
#         -pedantic  
CC_FLAGS=-D__TESTING__ \
		 -c			\
         -ansi      \
		 -pthread   \
		 -g			\
		 -std=c++17 \
		 -I"./sources/" \
		 $(CUSTOM_INCLUDE_PATH)
		 


# Flags for linker
#		 -W 		
#         -Wall      
#         -pedantic  
LK_FLAGS=-D__TESTING__\
         -ansi      \
		 -pthread   \
		 -g			\
		 -std=c++17 \
		 -I"./sources/" \
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
 
./objects/main/%.o: ../sources/%.cpp ../sources/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

./objects/tests/%.o: ./sources/%.cpp ./sources/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
 
./objects/tests/main.o: ./sources/main.cpp $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
	
clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rm -rf objects
 
.PHONY: all clean