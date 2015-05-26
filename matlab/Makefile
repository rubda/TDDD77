MATLAB_BIN = /Applications/MATLAB_R2015a.app/bin/matlab


ifdef MATLAB_BIN
PHONY: matlab
matlab:
	cd ../matrixlibrary; make
	cd ../quadopt; make
	$(MATLAB_BIN) -nodisplay -r "build; exit"
else
PHONY: matlab
matlab:
	@echo "MATLAB_BIN not defined in matlab/Makefile!"
endif
