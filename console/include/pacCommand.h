#ifndef PACCOMMAND_H
#define PACCOMMAND_H

#include "pacSingleton.h"

namespace pac {

class Command {
public:
  friend class CommandLib;
  /**
   * ctor.
   * @param name : ctor
   * @param ahName : handler name, don't set this if you need to build specific
   * handler for your command, override buildArgHandler instead.
   */
  Command(const std::string& name, const std::string& ahName = "");

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

  virtual void outputErrMessage(const std::string& args);

  virtual Command* clone() = 0;

  const std::string& getName() const { return mName; }
  void setName(const std::string& v) { mName = v; }

  /**
   * @param v : args and options string, must not start with space
   */
  virtual void setArgsAndOptions(const std::string& v);

  ArgHandler* getArgHandler() const;

  const std::string& getOptions() const { return mOptions; }
  void setOptions(const std::string& v) { mOptions = v; }
  const std::string& getArgs() const { return mArgs; }
  void setArgs(const std::string& v) { mArgs = v; }

protected:
  /**
   * Get default command argument handler name. It's "cmd_"+getName()
   * @return : default command argument handler name
   */
  std::string getDefAhName() { return "ahcmd_" + getName(); }

  virtual bool doExecute() = 0;

  /**
   * If your cmd has it's own arghandler, you should override this, it's name
   * should be getDefAhName(), you can use it later in other place if you want.
   * Otherwise you should setup mAhName in ctor.
   * @return : return true if you build cmd specific arg handler.
   */
  virtual bool buildArgHandler() { return false; };

  /**
   * check if has option
   * @param c : target option
   * @return : true if has option
   */
  bool hasOption(char c);

private:
  /**
   * Init cmd, build arghandler. Should be called by command lib when registered
   * at cmd lib
   * @return : this
   */
  Command* init();

protected:
  std::string mName;
  std::string mOptions;
  std::string mArgs;
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

class ManualCommand : public Command {
public:
  ManualCommand(const std::string& name, const std::string& ahName = "any");
  virtual void setArgsAndOptions(const std::string& v);
protected:
};
}

#endif /* PACCOMMAND_H */
