import os, glob

for infile in glob.glob('*.cpp'):
    f = open(infile)
    contents = f.readlines()
    f.close()
    o = open(infile,'w')
    for l in contents[:-1]:
        o.write(l.rstrip() + '\n')
    l = contents[-1].rstrip()
    o.write(l.rstrip())        
    o.close()

for infile in glob.glob('*.h'):
    f = open(infile)
    contents = f.readlines()
    f.close()
    o = open(infile,'w')
    for l in contents[:-1]:
        o.write(l.rstrip() + '\n')
    l = contents[-1].rstrip()
    o.write(l.rstrip())        
    o.close()