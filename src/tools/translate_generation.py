#!env python

import sys,os,fnmatch
from subprocess import call

translations_dir = os.getcwd() + "/translations"
if (len(sys.argv) == 2):
    translations_dir = sys.argv[1] + "/translations"

print("set translations dir:", translations_dir)

tslist = fnmatch.filter(os.listdir(translations_dir), '*.ts')

# This would print all the files and directories
for tsfile in tslist:
    tspath = translations_dir + "/" + tsfile
    print ("process", tspath)
    call(["lrelease", tspath])
