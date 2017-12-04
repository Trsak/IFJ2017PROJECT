all:
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=.. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=.. ./dokumentace.tex > NUL
	@bibtex ./dokumentace.aux
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=.. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=.. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=.. ./dokumentace.tex > NUL
	@rm ../dokumentace.aux ../dokumentace.log ../dokumentace.out ../dokumentace.toc NUL
