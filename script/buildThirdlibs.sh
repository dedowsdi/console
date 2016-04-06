#!/bin/bash
cd thirdlib
rm -rf *
ln -s /usr/local/source/ogre/ogre2.1/OgreMain OgreMain
ln -s /usr/local/source/ogre/ogre2.1/PlugIns/ParticleFX ParticleFX
ln -s /usr/local/source/mygui/mygui_ogre2.1/MyGUIEngine MyGUI
ln -s /usr/include/c++/4.8.4 gcc4.8.4
buildctags . 
