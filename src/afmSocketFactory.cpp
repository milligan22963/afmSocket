/**
 * @brief afmSocketFactory
 * 
 * Class for creating socket classes
 * 
 */

#include "afmSocketFactory.h"
#include "afmClientSocket.h"
#include "afmServerSocket.h"

namespace afm {
    namespace communications {
        AfmSocketFactory::AfmSocketFactory()
        {

        }

        AfmSocketFactory::~AfmSocketFactory()
        {

        }
        
        AfmSocketFactorySPtr AfmSocketFactory::getInstance()
        {
            static AfmSocketFactorySPtr pFactory = nullptr;

            if (pFactory == nullptr) {
                pFactory = std::make_shared<AfmSocketFactory>();
            }
            return pFactory;
        }

        IAfmSocketSPtr AfmSocketFactory::createSocket(eSocketType socketType)
        {
            IAfmSocketSPtr pSocket = nullptr;

            switch (socketType) {
                case eSocketType::eClientSocket: 
                {
                    pSocket = std::make_shared<AfmClientSocket>();
                }
                break;
                case eSocketType::eServerSocket: 
                {
                    pSocket = std::make_shared<AfmServerSocket>();
                }
                default:
                {
                    pSocket = nullptr;
                }
            }
            return pSocket;
        }
    }
}
