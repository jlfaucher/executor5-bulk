## How to generate the HTML files

#### README.html

```
rexx scripts/md2html4xtr README.md
```

#### rxunicode.cls.html

```
(cat rxunicode.cls | rexx scripts/oorexx2md >  rxunicode.cls.md) && rexx scripts/md2html4xtr rxunicode.cls.md
```

#### rxunicode.html

```
rexx scripts/md2md -fo rxunicode.md rxunicode1.md
BACKUP!
mv -f rxunicode1.md rxunicode.md

rexx scripts/md2html4xtr rxunicode.md
```

For test, generating the HTML using a local CSS not yet pushed to Github:

```
rexx scripts/md2html4xtr --css "file:///local/jlfaucher.github.io/css" rxunicode.md
```

#### unicode-escape-notations.html

Not generated from unicode-escape-notations.md.  
The AI provided both files.
