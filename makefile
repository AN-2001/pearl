cc := gcc
flags := -Wall -pedantic-errors -Werror
libs := -lm
obj := colour.o boilerplate.o gemWriter.o
proj := pearl

all: $(proj)


$(proj): $(proj).c $(obj) config.h
	$(cc) $< $(obj) $(flags) $(libs) -o $@

%.o : %.c %.h config.h
	$(cc) -c $< -o $@ $(flags) $(libs)

clean:
	rm $(proj) $(obj)
cleanPng:
	rm $(proj) $(obj) *.png
cleanAll:
	rm $(proj) $(obj) *.png *.mp4
encode:
	imageToVideo .
	
