#ifndef _CNR_TELNET_H_20170813_
#define _CNR_TELNET_H_20170813_
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "cnr/socket.h"

#define SESSION_BUFFER_SIZE 256
namespace cnr {

class EventDispather;
class Event;

struct CommandNode : public std::enable_shared_from_this<CommandNode> {
  enum NodeType {
    NODE_SUBNODE = 0,
    NODE_COMMAND = 1,
    NODE_NUMBER = 2,
    NODE_STRING = 3,
    NODE_ENUM = 4,
    NODE_EVALUE = 5,
  };

  static const char* toString(NodeType type);

  std::string name;
  NodeType type;
  std::weak_ptr<CommandNode> parent;
  std::map<std::string, std::shared_ptr<CommandNode>> childs;
  std::function<std::string(const std::string& value)> callback;
  std::string value;

  CommandNode(const std::string& name, NodeType type);

  ~CommandNode();
  bool addChild(std::shared_ptr<CommandNode> child);

  bool addEValues(const std::vector<std::string>& values);

  void invoke(Event* ev);

  static bool stringToBool(const std::string& value);
  static int stringToNumber(const std::string& value);
};

struct TelnetSession {
  // std::string hists[64];
  std::vector<std::string> hists;
  int hindex;
  char buf[SESSION_BUFFER_SIZE];
  int bindex;
  bool exit;

  TelnetSession();
};

class TelnetServer {
  DISALLOW_COPY_AND_ASSIGN(TelnetServer);
  Socket tcp_;
  EventDispather* base_;

  std::shared_ptr<CommandNode> commandRoot_;
  int currentFd_;

  std::map<int, TelnetSession> sessions_;

 public:
  TelnetServer(EventDispather* base, const char* port = "33922");

  virtual bool process(Event* ev, const char* buf, int len);

  void addCommandNode(std::shared_ptr<CommandNode> node);

 private:
  void onAccept(Event* ev);

  void onRead(Event* ev);

  bool processOneByte(TelnetSession& session, char byte);

  void initCommandTree();

  void initSession(Event* ev);

  void sendLine(Event* ev, int lastCount, const TelnetSession& session);

  int getCompletion(TelnetSession& session, bool printComdidates = false, Event* ev = NULL);
};

}  // namespace cnr

#endif  // _CNR_TELNET_H_20170813_