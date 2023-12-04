
run: main.exe
	@echo "Running the program"
	./main.exe

main.exe: forest_model.cpp
	@echo "Compiling the program"
	g++ -o main.exe forest_model.cpp -O3



clean:
	for %%X in (datos\*.bin) do del /Q "%%X"
	for %%X in (datos\*.csv) do del /Q "%%X"