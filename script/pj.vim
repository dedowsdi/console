call mycpp#util#addThirdLibTags('thirdlib')
"appName, makesure the same target exists in Makefile
let g:mycppDebugger = "lldb"
let g:mycppMakes = {
			\'default':'console',
			\'console':'',
			\'test':'test/test',
      \'testOgre':'ogre/test/testOgre',
      \'myguiConsole':'ogre/demo/mygui/myguiConsole'
			\}
let g:mycppBuildDir = "./build/Debug/"
let g:mycppClassPrefix='_PacExport'
let g:mycppPreRequisite=['pacConsolePreRequisite.h']
let g:mycppStable=['pacStable.h']
set path+=/usr/local/include/OGRE,/usr/local/include/MYGUI
