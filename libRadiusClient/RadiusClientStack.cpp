/*
 * This source file is part of the ace-radius library.  This code was
 * written by Alex Agranov in 2004-2009, and is covered by the BSD open source
 * license. Refer to the accompanying documentation for details on usage and
 * license.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#ifdef _MSC_VER
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif      

#include "RadiusClientStack.h"
#include "RadiusClientConnection.h"
#include "md5.h"

// ---------------------------
// Constructor
// ---------------------------
RadiusClientStack::RadiusClientStack(uint16_t p_port, const char * p_hostname) :
    m_nextPacketID(0),
    m_isValid(false)
{
    if (p_hostname != NULL)
        m_clientAddress.set(p_hostname, p_port);
    else
    {
        char l_hostname[200];
        gethostname(l_hostname,200);
        m_clientAddress.set(l_hostname, p_port);
    }

    // create UDP socket that we will use for communicating with RADIUS server
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket == -1)
        return;

    // set REUSE ADDRESS option
#ifdef _MSC_VER
    BOOL l_reuseAddr = TRUE;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&l_reuseAddr, sizeof(l_reuseAddr));
#else
    int l_reuseAddr = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &l_reuseAddr, sizeof(l_reuseAddr));
#endif

    // bind socket to given address and port
    struct sockaddr_in l_address;         // structure of address and port
    l_address.sin_family = AF_INET;
    l_address.sin_port = htons(p_port);
    if (p_hostname != NULL)
#if IN_ADDR_DEEPSTRUCT
        l_address.sin_addr.S_un.S_addr = m_clientAddress.getIP();
#else
        l_address.sin_addr.s_addr = m_clientAddress.getIP();
#endif
    else
#if IN_ADDR_DEEPSTRUCT
        l_address.sin_addr.S_un.S_addr = INADDR_ANY;
#else
        l_address.sin_addr.s_addr = INADDR_ANY;
#endif

    int l_rc = bind(m_socket, (struct sockaddr*)&l_address, sizeof(sockaddr_in));
    if (l_rc == -1)
        return;

    // make socket nonblocking
    /*
#ifdef _MSC_VER
    u_long l_blockMode = 1;
    ioctlsocket(m_socket, FIONBIO, &l_blockMode);
#else 
    int l_blockMode = 1;
    ioctl(m_socket, FIONBIO, &l_blockMode);
#endif
*/

    m_isValid = true;
}


// ---------------------------
// Destructor
// ---------------------------
RadiusClientStack::~RadiusClientStack()
{
    if (m_socket != -1)
#ifdef _MSC_VER
        closesocket(m_socket);
#else
        close(m_socket);
#endif

    delete m_connection;
}


// ---------------------------
// Add RADIUS server to work with
// ---------------------------
int RadiusClientStack::addServer(const char * p_hostname, uint16_t p_port, RadiusSecret p_secret, int p_responseTimeout, int p_retransmitCount)
{
    if (!isValid())
        return RC_FAIL;

    m_connection = new RadiusClientConnection(this, p_secret, p_hostname, p_port, p_responseTimeout, p_retransmitCount);
    return RC_SUCCESS;
}


// ---------------------------
// Send packet to the network
// ---------------------------
RadiusPacket * RadiusClientStack::sendPacket(RadiusPacket &p_packet, bool needResponse)
{
    RadiusPacket *l_response = m_connection->sendPacket(p_packet, needResponse);
    if (l_response != NULL)
        return l_response;

    return NULL;
}

// ---------------------------
// Get next ID for RADIUS packet
// ---------------------------
int RadiusClientStack::getNextPacketID()
{
    return m_nextPacketID++;
}


// ---------------------------
// Client stack was properly created?
// ---------------------------
bool RadiusClientStack::isValid()
{
    return m_isValid;
}

