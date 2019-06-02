fft:
	g++ fft.cpp -o finitefft
	g++ multiexp.cpp -o multiexp

clean:
	rm -rf finitefft multiexp
