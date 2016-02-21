#ifndef PACCOMMAND_H
#define PACCOMMAND_H

#include "pacSingleton.h"

namespace pac {

#define defCmdCom(type) \
  virtual Command* clone() { return new type(*this); }

class Command {
public:
  friend class CommandLib;
  /**
   * ctor.
   * @param name : ctor
   */
  Command(const std::string& name);

  /**
   * copy ctor, it doesn't copy mArgHandler.
   * @param rhs :
   */
  Command(const Command& rhs);

  virtual ~Command();

  /**
   * output prompt for argument
   * @remark :
   * @return :
   */
  virtual void prompt();

  /**
   * execute this cmd
   * @return : true or false
   */
  virtual bool execute();

  virtual bool doExecute() = 0;

  virtual Command* clone() = 0;

  const std::string& getName() const { return mName; }
  void setName(const std::string& v) { mName = v; }

  /**
   * Parse args and options
   * @param v : args and options string
   */
  void setArgsAndOptions(const std::string& v);

  /**
   * Get arg handler from arglib if it's 0.
   * @return : mArgHandler
   */
  ArgHandler* getArgHandler();

  const std::string& getAhName() const { return mAhName; }
  void setAhName(const std::string& v) { mAhName = v; }

  const std::string& getOptions() const { return mOptions; }
  void setOptions(const std::string& v) { mOptions = v; }
  const std::string& getArgs() const { return mArgs; }
  void setArgs(const std::string& v) { mArgs = v; }

private:
  /**
   * Init cmd, build arghandler. Should be called when registered
   * @return : this
   */
  Command* init();

protected:
  /**
   * Get default command argument handler name. It's "cmd_"+getName()
   * @return : default command argument handler name
   */
  std::string getDefAhName() { return "cmd_" + getName(); }

private:
  /**
   * Init argHandler, register it to arghandlerlib. Don't override this if you
   * use primitive arg handler.
   */
  virtual void buildArgHandler(){};

protected:
  std::string mName;
  std::string mOptions;
  std::string mArgs;
  std::string mAhName;  // argument ahandler name, it's cmd_{mName} by default

  ArgHandler* mArgHandler;
};

class CommandLib : public Singleton<CommandLib> {
public:
  typedef std::map<std::string, Command*> CmdMap;

  ~CommandLib();
  /**
   * Create command by command name
   * @param cmdName : command name
   * @return : newly created command or 0
   */
  Command* createCommand(const std::string& cmdName);

  /**
   * Register new command and it's argument handler.
   * @remark : don't release prototype by yourself
   * @param cmdProto : command prototype,
   */
  void registerCommand(Command* cmdProto);

  /**
   * init , register intrinsic commands.
   */
  void init();

  CmdMap::const_iterator beginCmdMapIterator() const;
  CmdMap::const_iterator endCmdMapIterator() const;

private:
  CmdMap mCmdMap;
};
}

#endif /* PACCOMMAND_H */
