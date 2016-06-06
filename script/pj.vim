"appName, makesure the same target exists in Makefile
let g:mycppDebugger = "gdb"
let g:mycppBuildDir = "./build/Debug/"
"let g:mycppMakes = {
			"\}
let g:mycppClassPrefix='_PacExport'
let g:mycppPreRequisite=['pacConsolePreRequisite.h']
let g:mycppStable=['pacStable.h']

call mycpp#addDependencies('dependencies')
