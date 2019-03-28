
#PROGNAME = eval-arith
PROGNAME = lam

CXXFLAGS = -ftemplate-depth-10000

OBJECTS = $(PROGNAME).o


run: $(PROGNAME).test


$(PROGNAME).test: $(PROGNAME)
	./$< | tee $@

$(PROGNAME): $(OBJECTS)
	g++ -o $@ $<


clean:
	rm -f $(OBJECTS) $(PROGNAME) $(PROGNAME).test
