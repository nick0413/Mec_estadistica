
run: main.exe
	@echo "Running the program"

	@for /f %%i in ('powershell -command "0..100 | %% {$_/100}"') do \
	.\main.exe %%i

run_p: main.exe
	@echo "Running the program"

	@for /f %%i in ('powershell -File sequence.ps1') do \
		.\main.exe %%i 0.0001

run_f: main.exe
	@echo "Running the program"

	@for /f %%i in ('powershell -File sequence2.ps1') do \
	.\main.exe 0.01 %%i

main.exe: forest_model.cpp
	@echo "Compiling the program"
	g++ -o main.exe forest_model.cpp -O3 -fopenmp



clean:
	for %%X in (datos\*.bin) do del /Q "%%X"
	for %%X in (datos\*.csv) do del /Q "%%X"