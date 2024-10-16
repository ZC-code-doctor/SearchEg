#ifndef __ACCEPTSERVER_H__
#define __ACCEPTSERVER_H__

#include <string>

using std::string;

class Acceptor;
class IOserver;

class AcceptServer
{
public:
    AcceptServer(Acceptor& accpetor,IOserver& ioserver);
    ~AcceptServer();
    void Start();
    void Stop();

private:
    Acceptor& _acceptor;
    IOserver& _ioserver;
    bool _isLoop;
};

#endif
