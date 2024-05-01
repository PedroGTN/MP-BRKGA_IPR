
def sol2tikz(sol_loc, tikz_loc, inst_loc):
    solfile = open(sol_loc, 'r')
    tikzfile = open(tikz_loc, 'w')
    instfile = open(inst_loc, 'r')

    tikzfile.write("\\documentclass[crop,tikz]{standalone}\n"
                    "\\usepackage{graphicx}\n"
                    "\\usepackage{tikz}\n\n"
                    "\\begin{document}\n"
                    "      \\begin{tikzpicture}[scale=0.4,cliente/.style={draw,circle,inner sep=2pt}, deposito/.style={draw,rectangle,inner sep=2pt}]\n")

    linhas = solfile.readlines()
    nodes = linhas[-21].split()
    drone_path = linhas[-19].split('|')
    node_locs = instfile.readlines()[3:]

   

    for n in nodes:
        if(n == '0'):
            tikzfile.write("        \\node[deposito] (0) at (" + node_locs[0][:-2].replace(" ", ",") + ") {Depot};\n")
        else:
            tikzfile.write("        \\node[cliente] (" + n + ") at (" + node_locs[int(n)][:-2].replace(" ", ",") + ") {" + n + "};\n")

    for d in drone_path[:-1]:
        if d.split(',')[1] in nodes:
            nodes.remove(d.split(',')[1])

    for i in range(len(nodes)-1):
        tikzfile.write("        \draw (" + nodes[i] + ") -- (" + nodes[i+1] + ");\n")

    tikzfile.write("        \draw (" + nodes[-1] + ") -- (" + nodes[0] + ");\n")

    for d in drone_path[:-1]:
        dnodes = d.split(',')
        tikzfile.write("        \draw[dashed] (" + dnodes[0] + ") -- (" + dnodes[1] + ");\n")
        tikzfile.write("        \draw[dashed] (" + dnodes[1] + ") -- (" + dnodes[2].replace("50", "0") + ");\n")
    

    tikzfile.write("      \end{tikzpicture}\n"
                    "\end{document}")
    return
