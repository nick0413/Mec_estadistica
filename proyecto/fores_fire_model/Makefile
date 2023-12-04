all:  run
	@echo "Corriendo el programa"

compile:
	@echo "Compiling the program"
	g++ -o main.exe forest_model.cpp -O3

run: compile
	@echo "Running the program"
	./main.exe

clean:
	for %%X in (datos\*.bin) do del /Q "%%X"
	for %%X in (datos\*.csv) do del /Q "%%X"