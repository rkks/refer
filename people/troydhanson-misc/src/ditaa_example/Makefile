all: sample1.png ditaa_asciidoc.html

sample1.png: sample1.txt
	java -jar ditaa0_9.jar sample1.txt

ditaa_asciidoc.html: ditaa_asciidoc.txt
	./handleimages.py $< | asciidoc -o $@ -
