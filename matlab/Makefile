MATLAB_BIN = 

PHONY: matlab
matlab:
	cd ../matrixlibrary; make
	cd ../quadopt; make
	$(MATLAB_BIN) -nodisplay -r "build; exit"

