#include "NetworkRequestInternal_p.h"

NetworkRequestInternal::NetworkRequestInternal(QObject* _parent, QNetworkCookieJar* _jar)
    : QObject(_parent), m_cookieJar(_jar), m_loadingTimeout(20000), m_request(new WebRequest()),
      m_networkRequest(nullptr)

{

}

NetworkRequestInternal::~NetworkRequestInternal()
{
    delete m_request;
}

