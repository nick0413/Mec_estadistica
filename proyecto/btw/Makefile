# wirte a makefile that cleans all csv files in the data folder
all:  run
	@echo "Cleaning all csv files in the data folder"

compile:
	@echo "Compiling the program"
	g++ -o main.exe BTW_model.cpp -O3 -fopenmp

run: compile
	@echo "Running the program"
	./main.exe

clean:
	for %%X in (datos\*.bin) do del /Q "%%X"