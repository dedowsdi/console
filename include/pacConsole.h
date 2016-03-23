#ifndef PACCONSOLE_H
#define PACCONSOLE_H

#include "pacSingleton.h"
#include "pacStringInterface.h"

namespace pac {

/**
 * Must be decoupled from ui and command.
 */
class Console : public Singleton<Console>, public StringInterface {
public:
  friend class RaiiConsoleBuffer;

  Console(ConsoleUI* ui);
  virtual ~Console();

  virtual void init();

  /**
   * Execute cmdLine. Add cmdLine to command history.
   * @param cmdLine : cmd line to be executed.It will get cmd line from ui if
   * it's empty.
   */
  bool execute(const std::string& cmdLine = "");

  /**
   * Prompt and complete for current cmd line .
   */
  void prompt();

  /**
   * Output to ui console or buffer. Call startBuffer before this if you want
   * to out put to buffer, call endBuffer after you finished output. Context
   * in buffer will be aligned automatically. Buffer is only used for type 1.
   * @param s : output content
   * @param type : 1 stdout, 2 stderr
   * @return : Console&
   */
  Console& output(const std::string& s, int type = 1);

  Console& outputLine(const std::string& s, int type = 1);

  /**
   * Complete current typing.
   * @param s : string to be added after current cursor
   */
  Console& complete(const std::string& s);

  /**
   * Change current working dir
   * @param dir : target dir
   */
  void setCwd(AbsDir* dir);
  AbsDir* getCwd() { return mDir; }

  /**
   * Buffer output, to be aligned later. This function will reset buffer. Use
   * RaiiConsoleBuffer to make suer endBuffer get called after this.
   */
  void startBuffer();
  /**
   * flash buffer
   */
  void endBuffer();

  /**
   * Roll command history
   * @param backWard : roll back or roll front
   */
  void rollCommand(bool backWard = true);

  /**
   * Clean all temporary dirs.
   */
  void cleanTempDirs();

  ConsoleUI* getUi() const { return mUi; }
  void setUi(ConsoleUI* v) { mUi = v; }

  bool isActive();
  void setActive(bool b);
  void toggleActive();
  ConsolePattern* getPattern() const { return mPattern; }

  void resize();
  

protected:
  // set up console pattern
  virtual void initConoslePattern();
  // set up cmd history
  virtual void initCmdHistory();
  // set up dir system
  virtual void initDir();
  //set up arg handlers
  virtual void initArghandler();
  //set up commands 
  virtual void initCommand();

private:
  /**
   * Prompt for command name .
   * @param cmdLine :
   */
  void promptCommandName(const std::string& cmdName);

  /**
   * When you hit tab or enter in term, there will be a record of cwd and
   * command line. This is used to fake that.
   * @param cmdLine : current command line
   */
  void fakeOutputDirAndCmd(const std::string& cmdLine);

  void appendBuffer(const std::string& v);

  void cleanTempDir(AbsDir* dir);


private:
  int mIsBuffering;

  AbsDir* mDir;
  ConsoleUI* mUi;
  ConsolePattern* mPattern;
  CmdHistory* mCmdHistory;
  StringVector mBuffer;
};

/**
 * RAII of console buffer
 */
class RaiiConsoleBuffer {
public:
  RaiiConsoleBuffer();
  ~RaiiConsoleBuffer();
};
}

#endif /* PACCONSOLE_H */
