main.pdf: start
	docker cp . latexcomp:/latex
	docker exec latexcomp latexmk -pdf -interaction=nonstopmode main.tex
	docker cp latexcomp:/latex/main.pdf main.pdf

clean: start
	docker exec latexcomp rm -r /latex

start:
	@docker ps | grep -q latexcomp || { echo -e "\033[0;31m!! Latex compiler is not running\033[0m" && exit 1; }
