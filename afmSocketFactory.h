/**
 * @brief afmSocketFactory
 * 
 * Class for creating socket classes
 * 
 */

#ifndef _H_AFM_SOCKET_FACTORY
#define _H_AFM_SOCKET_FACTORY

#include <memory>
#include "IAfmSocket.h"

namespace afm {
    namespace communications {
        
        enum eSocketType {
            eClientSocket,
            eServerSocket,
            eEndSocketTypes
        };

        class AfmSocketFactory;

        using AfmSocketFactorySPtr = std::shared_ptr<AfmSocketFactory>;

        class AfmSocketFactory
        {
            public:
                AfmSocketFactory();
                virtual ~AfmSocketFactory();
                
                static AfmSocketFactorySPtr getInstance();

                IAfmSocketSPtr createSocket(eSocketType socketType);
        };
    }
}
#endif