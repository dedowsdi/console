call util#addThirdLibTags('thirdlib')
"appName, makesure the same target exists in Makefile
let g:mycppAppName='console'
let g:mycppClassPrefix='_PacExport'
let g:mycppPreRequisite=['pacConsolePreRequisite.h']
let g:mycppStable=['pacStable.h']
