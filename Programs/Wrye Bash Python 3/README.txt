This will be my work on getting Wrye Bash ported to Python 3.

I'm doing a seperate folder rather than a branch for a few reasons:

1) I plan on refactoring as I go, splitting the thousands of lines files
   into many small files.

2) Enough will change simply due to the changes in Python 3 that it won't
   be worth maintaining it as a branch.

3) Wrye Bash for Python 3 will have pretty strict requirements.  Mostly:
   a) wxPython - using wxPhoenix, which is still in dev, and cannot be
      downloaded as an installer.  wxPhoenix requires Python 3.2 (can't
      be higher).  I may try an experimental build at some point for
      Python 3.3, as the advantages of Python 3.3 over 3.2 are pretty good.
   b) py2exe - Pretty much non-existant for Python 3.  So I'll probably have
      to either port py2exe to Python 3, or try an alternate solution (freeze
      or something)
