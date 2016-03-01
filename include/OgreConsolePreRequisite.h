#ifndef OGRECONSOLEPREREQUISITE_H
#define OGRECONSOLEPREREQUISITE_H

#include "pacConsolePreRequisite.h"
#include "OgrePrerequisites.h"

namespace pac {
class MolDir;
class MovableSI;
class LightSI;
class CameraSI;
class AthCmd;
class Dthmo;
class EdmoCmd;
class EdndCmd;
class LsmatCmd;
class LsmoCmd;
class Lsnd;
}

#define sgOgreConsole static_cast<OgreConsole&>(pac::Console::getSingleton())

#endif /* OGRECONSOLEPREREQUISITE_H */
