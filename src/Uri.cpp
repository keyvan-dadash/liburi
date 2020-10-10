


#include "../include/Uri/Uri.hpp"

namespace Uri {

    Uri::~Uri() 
    {

    }

    Uri::Uri() : comp_(new uriComponents) 
    {

    }


    bool Uri::parseFromString(std::string uriStirng) 
    {
        this->comp_->uri = uriStirng;
        this->extractSchema();
        this->extractUserInfo();
        this->extractHost();
        this->extractPort();
        return true;
    }

    void Uri::extractSchema() 
    {
        std::string uri = this->comp_->uri;

        size_t delemiterChar = uri.find(':');
        this->comp_->schema = uri.substr(0, delemiterChar);
    }

    void Uri::extractUserInfo() 
    {
        std::string uri = this->comp_->uri;

        size_t endOfSlashSlash = this->getSchema().size() + 3;

        size_t atSignPtr = uri.find('@');
        if (atSignPtr == std::string::npos) {
            this->comp_->hasUserInfo = false;
            return;
        }
        this->comp_->hasUserInfo = true;

        this->comp_->userInfo = uri.substr(endOfSlashSlash, (atSignPtr - endOfSlashSlash));
    }

    void Uri::extractHost() 
    {
        std::string uri = this->comp_->uri;

        bool hasHost;

        size_t startOfSlashSlash = this->getSchema().size() + 1;
        
        uri.substr(startOfSlashSlash, 2).compare("//") == 0 ? hasHost = true : hasHost = false ;

        if (!hasHost) {
            this->comp_->hastHost = false;
            return;
        }
        this->comp_->hastHost = true;

        //must refac this bcs it is dublicate
        if (this->comp_->hasUserInfo) {
            size_t atSignPtr = uri.find('@');

            size_t endOfHost = uri.substr(atSignPtr + 1).find(':');
            if (endOfHost == std::string::npos) {
                endOfHost = uri.substr(atSignPtr + 1).find('/');
                if (endOfHost == std::string::npos)
                    endOfHost = uri.size();
            }
            endOfHost += atSignPtr;
            this->comp_->host = uri.substr(atSignPtr + 1, (endOfHost - atSignPtr));
        } else {
            size_t endOfSlashSlash = startOfSlashSlash + 2;

            size_t endOfHost = uri.substr(endOfSlashSlash).find(':');
            if (endOfHost == std::string::npos) {
                endOfHost = uri.substr(endOfSlashSlash).find('/');
                if (endOfHost == std::string::npos)
                    endOfHost = uri.size();
            }
            endOfHost += endOfSlashSlash;
            this->comp_->host = uri.substr(endOfSlashSlash, (endOfHost - endOfSlashSlash));
        }
    }

    void Uri::extractPort() 
    {
        std::string uri = this->comp_->uri;

        size_t colonAfterSchema = uri.find(":");
        size_t colonBeforePortAfterSchmea = uri.find(":", colonAfterSchema + 1);

        if (colonBeforePortAfterSchmea == std::string::npos) {
            this->comp_->hasPort = false;
            this->comp_->port = DEFAULT_PORT;
            return;
        }
        this->comp_->hasPort = true;

        size_t endOfPort = uri.substr(colonBeforePortAfterSchmea + 1).find("/");
        if (endOfPort == std::string::npos) {
            endOfPort = uri.size();
        } else {
            endOfPort += colonBeforePortAfterSchmea;
        }
        this->comp_->port = std::atoi(uri.substr(colonBeforePortAfterSchmea + 1, (endOfPort - colonBeforePortAfterSchmea)).c_str());
    }

    std::string Uri::getSchema() const 
    {
        return  this->comp_->schema;
    }

    std::string Uri::getUserInfo() const 
    {
        return this->comp_->userInfo;
    }

    std::string Uri::getHost() const 
    {
        return this->comp_->host;
    }

    uint16_t Uri::getPort() const 
    {
        return this->comp_->port;
    }

    std::vector< std::string > Uri::getPath() const 
    {
        return std::vector< std::string > { "", "ops" };
    }





}