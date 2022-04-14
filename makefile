CC=clang
CFLAGS=-g
ODIR=obj

LIBRARY_RESULT_FILE_NAME = four_matrix_to_diffusion_asymptotic.so
TEST_C_FILE = lib_test.c 
FILE_NAMES = c_part get_rab_from_constans_and_x trapezoidal_rule 

MESCHACH_DIR = Meschach
MESCHACH_LIBRARY = $(MESCHACH_DIR)/libmeschach.a

LIBS=-lm -L./$(MESCHACH_DIR) -lmeschach

DEPS = $(patsubst %,%.h,$(FILE_NAMES))

_OBJ = $(patsubst %,%.o,$(FILE_NAMES))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

FLAGS_FOR_DYNAMIC_LIBRARY = -shared -Wl,-soname,$(LIBRARY_RESULT_FILE_NAME)


$(MESCHACH_LIBRARY): $(MESCHACH_DIR)
	cd $(MESCHACH_DIR) && cmake . -DCOMPLEX_OPTION=NO -DSPARSE_OPTION=NO 
	$(MAKE) -C $(MESCHACH_DIR)

$(ODIR)/%.o: %.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

library: $(OBJ) $(MESCHACH_LIBRARY)
	$(CC) -o $@ $^ $(CFLAGS) $(FLAGS_FOR_DYNAMIC_LIBRARY) $(LIBS) 

.PHONY: clean test

test: $(OBJ)
	$(CC) -o $@ $(TEST_C_FILE) $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
