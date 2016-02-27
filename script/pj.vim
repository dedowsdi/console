call util#addThirdLibTags('thirdlib')
"appName, makesure the same target exists in Makefile
let g:mycppMakes = {
			\'default':'console',
			\'console':'',
			\'test':'test/test',
			\}
let g:mycppBuildDir = "build"
let g:mycppClassPrefix='_PacExport'
let g:mycppPreRequisite=['pacConsolePreRequisite.h']
let g:mycppStable=['pacStable.h']
