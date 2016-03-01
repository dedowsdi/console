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
 * get reParam ("1")
 * get path ("2")
 * get path reParam ("3")
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
  void outputProperties(AbsDir* dir, const SVCIter beg, const SVCIter end);
};

/**
 * serialize dir
 * sz ("0")
 * sz path ("1")
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
