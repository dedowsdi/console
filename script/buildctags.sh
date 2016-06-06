#!/bin/bash
opts="-R --sort=foldcase --c++-kinds=+p --c-kinds=+p --exclude=build --exclude=cmake --exclude=CMake"
ctags --links=no $opts .
ctags --links=no $opts dependencies/Ogre/* dependencies/MyGUI/*
