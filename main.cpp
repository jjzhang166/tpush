#include <stdio.h>
#include <string>

#include <signal.h>

#include "log.h"

#include "address.h"
#include "tcpserver.h"

#include "connection.h"

#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "misc.h"

using namespace std;
using namespace tpush;
using namespace std::tr1::placeholders;

void sigAction(TcpServer* pServer, int signum)
{
    printf("signum %d\n", signum);

    pServer->stop();
}

typedef std::tr1::shared_ptr<Connection> ConnectionPtr_t;
typedef std::tr1::shared_ptr<HttpRequest> HttpRequestPtr_t;

void onHandler(const ConnectionPtr_t& conn, const HttpRequestPtr_t& request)
{
    //LOG_INFO("request url %s", request->url.c_str());
    map<string, string> headers = request->headers;
    for(map<string, string>::iterator it = headers.begin();
        it != headers.end(); ++it)
    {
        //LOG_INFO("request header %s: %s", it->first.c_str(), it->second.c_str());    
    }

    //LOG_INFO("request body %s", request->body.c_str());

    HttpResponse resp;
    resp.statusCode = 200;
    resp.headers["Content-Type"] = "text/html";
    resp.headers["Connection"] = "keep-alive";
    resp.body = "Hello World";

    conn->send(resp.dump());
}

int main()
{
    //Log::rootLog().setLevel(Log::ERROR);
     
    TcpServer s(1, 1, 100);
    HttpServer httpd(&s);

    httpd.setRequestCallback(std::tr1::bind(&onHandler, _1, _2));

    httpd.listen(Address(11181));

    s.addSignal(SIGINT, std::tr1::bind(sigAction, &s, std::tr1::placeholders::_1));

    LOG_INFO("start tcp server");

    s.start();

    LOG_INFO("stop server");

    return 0;
}   
