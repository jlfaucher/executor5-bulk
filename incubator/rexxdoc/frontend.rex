use strict arg file
parser = .FileParser~new(file)
parser~parse
say parser~tree

out = .stdout
out = .stream~new("doc.html")~~command("open replace")

out~say("<html><head><title>")
out~say(parser~tree~name)
out~say("</title></head><body>")
printFile(parser~tree,out)
out~say("<h2>Required Files</h2>")
do r over parser~tree~requires
  out~say("<strong>"r~getFile~name"</strong><br>")
  printFile(r~getFile,out)
end

out~say("</body></html>")
::REQUIRES "rexxdoc.cls"

::ROUTINE doc2string
  use arg doc
  if doc~class \= .string then do
    accu = ""
    do d over doc
      if d~strip = "" then accu||="<br>"
      else accu||=" "d
    end
    return accu"<br>"
  end
  else return doc"<br>"
::ROUTINE printFile
  use arg tree, out
  out~say(doc2string(tree~doc))
      out~say("<pre>")
      do s over tree~source~source
        if s = "" then iterate
        out~say(s)
      end
      out~say("</pre>")
  classes = tree~classes
  out~say("<h1>Classes</h1>")
  do o over classes  
    out~say('<a href="#'o~name'">'o~name||"</a><br>")
  end
  do o over classes
    out~say('<a name="'o~name'"/><h2>'o~name"</h2>")
    out~say(doc2string(o~doc))
    out~say("<h3>Methods</h3>")
    methods = classes[o]~methods
    do p over methods
      out~say("<strong>"p~name"</strong><br>")
      out~say(doc2string(p~doc))
      out~say("<pre>")
      do s over p~source~source
        if s = "" then iterate
        out~say(s)
      end
      out~say("</pre>")
    end
  end
return ""