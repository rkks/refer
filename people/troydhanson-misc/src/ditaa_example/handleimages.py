#!/usr/bin/env python
# this script is from Ivo's blog:
# http://vanderwijk.info/2009/4/23/full-text-based-document-generation-using-asciidoc-and-ditaa
import sys
import re
import subprocess

header = re.compile("\[ASCIIART\s+(\S+?)\s*\((.*?)\)?\]")

def main(document):
    parsed = []
    parsing = False
    filename = ""
    worklist = {}
    for line in document:
        if line.strip().startswith("[ASCIIART"):
            filename, flags = header.match(line).groups()
            if flags:
                flags = "[%s]" % flags
            else:
                flags = ""
            parsed.append("image:%s%s\n\n" % (filename, flags))
            parsing = True
            worklist[filename] = ""
        elif line.strip() == "[TRAIICSA]":
            a = open("/tmp/asciiart.txt", "w")
            a.write(worklist[filename])
            a.close()
            parsing = False
            job = subprocess.Popen(["java", "-jar", "ditaa0_9.jar", "-o", "/tmp/asciiart.txt", filename], stdout=subprocess.PIPE)
            job.wait()
        elif parsing:
            worklist[filename] += line
        else:
            parsed.append(line)

    print "".join(parsed)

if __name__ == "__main__":
    if len(sys.argv) == 2:
        document = open(sys.argv[1], "r").readlines()
    else:
        document = sys.stdin.readlines()
    main(document)
