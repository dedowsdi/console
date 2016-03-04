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
  virtual Command* clone() { return new LsCmd(*this); }
  LsCmd();

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();

private:
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
  PwdCmd();
  virtual Command* clone() { return new PwdCmd(*this); }

protected:
  virtual bool doExecute();
};

/**
 * ctd
 * clean tempory dirs
 */
class _PacExport CtdCmd : public Command {
public:
  CtdCmd();
  virtual Command* clone() { return new CtdCmd(*this); }

protected:
  virtual bool doExecute();
};

/**
 * cd path
 * change current working directory
 */
class _PacExport CdCmd : public Command {
public:
  CdCmd();
  virtual Command* clone() { return new CdCmd(*this); }

protected:
  virtual bool doExecute();
};

/**
 * set param value
 * set path param value
 */
class _PacExport SetCmd : public Command {
public:
  SetCmd();
  virtual Command* clone() { return new SetCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};

/**
 * get ("0")
 * get param ("1")
 * get ltl_regex regex("2")
 * get path ("3")
 * get path param ("4")
 * get path ltl_regex regex("5")
 *
 * list properties
 */
class _PacExport GetCmd : public Command {
public:
  GetCmd();
  virtual Command* clone() { return new GetCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();

private:
  /**
   * helper to output dir param
   * @param dir : target dir
   * @param param : param name, don't set regex if you want to use this
   * @param reExp : regex expession, don't set param if you want to use this
   * @return :
   */
  void outputProperties(AbsDir* dir, const std::string& param = "",
      const std::string& reExp = "");
};

/**
 * serialize dir to file, it's a recursive opration by default, use -R if you
 * don't want to block recursive
 * sz [-R] id ("0")
 * sz [-R] id path ("1")
 */
class _PacExport SzCmd : public Command {
public:
  SzCmd();
  virtual Command* clone() { return new SzCmd(*this); }

protected:
  virtual bool doExecute();
  virtual bool buildArgHandler();
};
};

#endif /* PACINTRINSICCMD_H */
