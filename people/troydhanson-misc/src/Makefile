all: warning index.html

warning:
	@if [ -z "`type -t asciidoc`" ]; then \
	echo "this Makefile is for building the documentation"; \
	false; else true; fi

index.html: index.txt sslogo.txt
	asciidoc $<

install: index.html 
	cp index.html ${PAGEROOT}/misc/index.html
	cp sslogo.png ${PAGEROOT}/misc/sslogo.png
