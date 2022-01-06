####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES: all				
.PHONY: clean all dox splint

####################################################################
# Path Definitions                                                 #
####################################################################
OUT_DIR     = ./output
OBJ_DIR     = $(OUT_DIR)/build
EXE_DIR     = $(OUT_DIR)/exe
DOX_DIR     = $(OUT_DIR)/docu
SRC_DIR     = ./src
INCLUDE_DIR = ./include

####################################################################
# Flag Definitions                                                 #
####################################################################
FLAGS = -lm -fopenmp -Wall -g
# SPLINTFLAGS = +standard -skipposixheaders -mustfreeonly -likelybool -temptrans -usedef

SPLINTFLAGS = +posixlib +standard -mustfreeonly -likelybool \
			  -temptrans -nullstate -mustdefine -compdef -compdestroy \
			  -dependenttrans -noeffect
## -warnposix -preproc 
INCLUDEPATHS =-I$(INCLUDE_DIR)

####################################################################
# Create build and ouput directory								   #
####################################################################
$(OBJ_DIR):
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)
	@echo "Created build directory."


####################################################################
# Build instructions			 								   #
####################################################################
all: $(OBJ_DIR) $(EXE_DIR)/main 

$(EXE_DIR)/main: $(OBJ_DIR)/stencil.o

$(OBJ_DIR)/stencil.o: $(SRC_DIR)/stencil.cpp 
	g++ $(FLAGS) $(INCLUDEPATHS) $(SRC_DIR)/stencil.cpp -o $(OBJ_DIR)/stencil 

dox:
	mkdir -p $(DOX_DIR)
	@echo "Created doxygen directory."

	doxygen ./doc/stencil.dox

	google-chrome ./output/docu/html/index.html
	
splint:
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/stencil.cpp

clean:
	rm -rf $(OBJ_DIR) $(DOX_DIR) $(EXE_DIR) $(OUT_DIR)