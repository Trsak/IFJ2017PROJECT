all:
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=. ./dokumentace.tex > NUL
	@bibtex ./dokumentace.aux > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=. ./dokumentace.tex > NUL
	@pdflatex -file-line-error -interaction=nonstopmode -synctex=0 -output-format=pdf -output-directory=. ./dokumentace.tex > NUL
	@mv ./dokumentace.pdf ../
	@rm ./dokumentace.aux ./dokumentace.log ./dokumentace.blg ./dokumentace.bbl ./dokumentace.out ./dokumentace.toc NUL