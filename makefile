CC=gcc
CFLAGS= -O3 -fPIC
ODIR=obj

LIBRARY_RESULT_FILE_NAME = libfour_matricies_to_diffusion_asymptotic.a
TEST_C_FILE = lib_test.c 
FILE_NAMES = four_matricies_to_diffusion_asymptotic get_rab_from_constans_and_x trapezoidal_rule 

MESCHACH_DIR = Meschach
MESCHACH_LIBRARY = $(MESCHACH_DIR)/libmeschach.a

LIBS=-lm -L./$(MESCHACH_DIR) -lmeschach

DEPS = $(patsubst %,%.h,$(FILE_NAMES))

_OBJ = $(patsubst %,%.o,$(FILE_NAMES))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TEMP_OBJ = temporary

library: $(OBJ) $(MESCHACH_LIBRARY)
	cp -r $(ODIR) $(TEMP_OBJ)
	cd $(TEMP_OBJ) && ar xv ../$(MESCHACH_LIBRARY)
#	cd $(TEMP_OBJ) && ar xv /usr/lib/x86_64-linux-gnu/libm.a 
	cd $(TEMP_OBJ) && ar rsc $(LIBRARY_RESULT_FILE_NAME) *
	cp $(TEMP_OBJ)/$(LIBRARY_RESULT_FILE_NAME) ./
	rm -r $(TEMP_OBJ)

$(MESCHACH_LIBRARY): $(MESCHACH_DIR)
	cd $(MESCHACH_DIR) && cmake . -DCOMPLEX_OPTION=NO -DSPARSE_OPTION=NO -DCMAKE_C_FLAGS=-fPIC
	$(MAKE) -C $(MESCHACH_DIR)

$(ODIR)/%.o: %.c
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean test library

test: $(OBJ) $(MESCHACH_LIBRARY)
#	$(CC) -o $@ $(TEST_C_FILE) *.c $(CFLAGS) $(LIBS)
	$(CC) -o $@ $(TEST_C_FILE) $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
