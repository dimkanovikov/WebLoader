#include "networkrequestinternal.h"

NetworkRequestInternal::NetworkRequestInternal(QObject * parent, QNetworkCookieJar * jar)
    : QObject(parent), cookieJar(jar), loadingTimeout(20000), m_request(new WebRequest())

{

}

NetworkRequestInternal::~NetworkRequestInternal()
{
    delete m_request;
}

