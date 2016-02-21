#ifndef PACINTRINSICCMD_H
#define PACINTRINSICCMD_H

#include "pacCommand.h"

namespace pac {

/**
 * ls path*
 * list dir under path
 */
class _PacExport LsCmd : public Command {
public:
  defCmdCom(LsCmd) LsCmd();
  virtual bool doExecute();

private:
  virtual void buildArgHandler();
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

private:
  virtual void buildArgHandler();
};

/**
 * cd path
 * change current working directory
 */
class _PacExport CdCmd : public Command {
public:
  defCmdCom(CdCmd) CdCmd();
  virtual bool doExecute();

private:
  virtual void buildArgHandler();
};

/**
 * set name value
 * set property name of current working directory
 * value can be multiple items separated by space
 */
class _PacExport SetCmd : public Command {
public:
  defCmdCom(SetCmd) SetCmd();
  virtual bool doExecute();

private:
  virtual void buildArgHandler();
};

/**
 * lp path*
 * list properties of specific dirs
 * @param lp :
 * @param lpCd :
 * @return :
 */
class _PacExport LpCmd : public Command {
public:
  defCmdCom(LpCmd) LpCmd();
  virtual bool doExecute();

private:
  virtual void buildArgHandler();
  void outputProperties(AbsDir* dir);
};
};

#endif /* PACINTRINSICCMD_H */
