#ifndef PACINTRINSICCMD_H
#define PACINTRINSICCMD_H

#include "pacCommand.h"

namespace pac {

/**
 * ls ("0") 
 * ls path+ ("1")
 * list dir under path
 */
class _PacExport LsCmd : public Command {
public:
  defCmdCom(LsCmd) LsCmd();
  virtual bool doExecute();

private:
  virtual bool buildArgHandler();
  /**
   * output child dir under dir
   * @param dir : working directory
   */
  void outputChildren(AbsDir* dir);
};

/**
 * pwd
 * print current working directory
 */
class _PacExport PwdCmd : public Command {
public:
  defCmdCom(PwdCmd) PwdCmd();
  virtual bool doExecute();

};

/**
 * cd path
 * change current working directory
 */
class _PacExport CdCmd : public Command {
public:
  defCmdCom(CdCmd) CdCmd();
  virtual bool doExecute();
};

/**
 * set param value
 * set path param value
 */
class _PacExport SetCmd : public Command {
public:
  defCmdCom(SetCmd) SetCmd();
  virtual bool doExecute();

private:
  virtual bool buildArgHandler();
};

/**
 * lp ("0")
 * lp param ("1")
 * lp path ("2")
 * lp path param ("3")
 *
 * list properties 
 */
class _PacExport LpCmd : public Command {
public:
  defCmdCom(LpCmd) LpCmd();
  virtual bool doExecute();

private:
  virtual bool buildArgHandler();
  void outputProperties(AbsDir* dir);
};
};

#endif /* PACINTRINSICCMD_H */
