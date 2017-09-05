C=g++

ifeq ($(DEBUG),yes)
	CXXFLAGS=-Wall -g -std=c++11
	LDFLAGS=-Wall -g
else
	CXXFLAGS=-Wall -std=c++11
	LDFLAGS=-Wall
endif

AR=ar
ARFLAGS=rcs

INCPATH=inc
SRCPATH=src
OBJPATH=obj
LIBPATH=.

INC=$(wildcard $(INCPATH)/*.h)
SOURCES:=$(wildcard $(SRCPATH)/*.cpp)
OBJ:=$(patsubst $(SRCPATH)/%.cpp,$(OBJPATH)/%.o,$(SOURCES))
OUT=$(LIBPATH)/libdatab.a

INCLUDES=-I ./$(INCPATH)

default: $(OUT)

$(OUT): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

$(OBJPATH)/%.o: $(SRCPATH)/%.cpp $(INC)
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean cleanall

clean:
	rm -f $(OBJPATH)/*.o

cleanall: clean
	rm -f $(OUT)
