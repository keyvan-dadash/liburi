


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
        this->resetComponents();
        this->comp_->uri = uriStirng;
        return this->parse();
    }

    void Uri::resetComponents() 
    {
        this->comp_.reset(new uriComponents);
    }

    bool Uri::parse() 
    {
        try {
            this->extractSchema();
            this->extractUserInfo();
            this->extractHost();
            this->extractPort();
            this->extractPath();
            this->extractQueryParams();
            this->extractFragment();
            this->comp_->isValid = true;
        } 
        catch (std::exception &ex) {
            //std::cerr << ex.what() << std::endl;
            this->comp_->isValid = false;
        }
        return this->comp_->isValid;
    }

    void Uri::extractSchema() 
    {
        std::string uri = this->comp_->uri;

        size_t delemiterChar = uri.find(':');
        size_t slashSlash = uri.find("//");
        if (slashSlash == std::string::npos && delemiterChar == std::string::npos) {
            this->comp_->hasSchema = false;
            this->comp_->isRelative = true;
            return;
        }
        this->comp_->schema = uri.substr(0, delemiterChar);
        this->comp_->isRelative = false;
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
        std::string port = uri.substr(colonBeforePortAfterSchmea + 1, (endOfPort - colonBeforePortAfterSchmea));
        int portStartIndex = 0;
        int prefixZeroIndex = 0;

        //check if port is positive and have no invalid char
        for (size_t i = 0; i < port.size(); i++)
        {
            if (port.at(i) > '9' || port.at(i) < '0') {
                this->comp_->hasPort = false;
                throw std::runtime_error(std::string("the port is invalid"));
            }
            if (prefixZeroIndex == i && port.at(i) == '0') {
                portStartIndex++;
                prefixZeroIndex++;
            }
            
        }

        //delete prefix 0
        port = port.substr(portStartIndex);

        if (port.size() > 5 || std::atoi(port.c_str()) > 65535) {
            this->comp_->hasPort = false;
            throw std::runtime_error(std::string("port range is between 0 and 65535"));
        }
        
        this->comp_->port = std::atoi(port.c_str());
    }

    void Uri::extractPath() 
    {
        std::string uri = this->comp_->uri;

        if (this->comp_->hastHost) {
            size_t endOfSlashSlash = uri.find("//") + 2;
            std::string stringAfterSlashSlash = uri.substr(endOfSlashSlash);
            size_t pos = stringAfterSlashSlash.find('/');
            while (pos != std::string::npos) {
                size_t endOfLocalPath = stringAfterSlashSlash.find('/', pos + 1);

                if (endOfLocalPath == std::string::npos)
                    endOfLocalPath = stringAfterSlashSlash.find('?');
                if (endOfLocalPath == std::string::npos)
                    endOfLocalPath = stringAfterSlashSlash.size();

                this->comp_->path.push_back(stringAfterSlashSlash.substr(pos + 1, (endOfLocalPath - pos - 1)));
                pos = stringAfterSlashSlash.find('/', pos + 1);
            }
        } else {
            size_t schemaEnd = uri.find(":") + 1;
            size_t pathEnd = uri.size();
            this->comp_->path.push_back(uri.substr(schemaEnd, (pathEnd - schemaEnd)));
        }
        if (this->comp_->path.size() >= 1) 
            this->comp_->hasPath = true;
        else 
            this->comp_->hasPath = false;
    }

    void Uri::extractQueryParams()
    {
        std::string uri = this->comp_->uri;
        size_t startOfQueryParam = uri.find('?');
        
        std::vector< std::string > paramsWithKeyAndValue;
        while(startOfQueryParam != std::string::npos) {
            size_t endOfCurQueryParams = uri.find('&', startOfQueryParam + 1);
            if (endOfCurQueryParams == std::string::npos) 
                endOfCurQueryParams = uri.find('#');
            if (endOfCurQueryParams == std::string::npos) 
                endOfCurQueryParams = uri.size();
            paramsWithKeyAndValue.push_back(uri.substr(startOfQueryParam + 1, (endOfCurQueryParams - startOfQueryParam - 1)));
            startOfQueryParam = uri.find('&', startOfQueryParam + 1);
        }

        std::map< std::string, std::string > queryParams;
        for(auto &paramWithKV : paramsWithKeyAndValue) {
            size_t startOfValue = paramWithKV.find('=') + 1;
            std::string value;
            if (startOfValue == std::string::npos) 
                value = "";
            else
                value = paramWithKV.substr(startOfValue, (paramWithKV.size() - startOfValue));
            
            size_t endOfKey = startOfValue - 1;
            queryParams[paramWithKV.substr(0, endOfKey)] = value;
        }

        this->comp_->queryParams = queryParams;
        if (this->comp_->queryParams.size() >= 1) 
            this->comp_->hasQueryParams = true;
        else 
            this->comp_->hasQueryParams = false;
    }

    void Uri::extractFragment()
    {
        std::string uri = this->comp_->uri;

        size_t startOfFragment = uri.find('#');
        if (startOfFragment == std::string::npos) {
            this->comp_->hasFragment = false;
            return;
        }
        this->comp_->hasFragment = true;

        this->comp_->fragment = uri.substr(startOfFragment + 1, (uri.size() - startOfFragment - 1));
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
        return this->comp_->path;
    }

    std::map< std::string, std::string > Uri::getQueryParams() const
    {
        return this->comp_->queryParams;
    }

    std::string Uri::getFragment() const
    {
        return this->comp_->fragment;
    }

    bool Uri::isValid() const
    {
        return this->comp_->isValid;
    }

    bool Uri::hasSchema() const
    {
        return this->comp_->hasSchema;
    }

    bool Uri::hasUserInfo() const
    {
        return this->comp_->hasUserInfo;
    }

    bool Uri::hasHost() const
    {
        return this->comp_->hastHost;
    }

    bool Uri::hasPort() const
    {
        return this->comp_->hasPort;
    }

    bool Uri::hasPath() const
    {
        return this->comp_->hasPath;
    }

    bool Uri::hasQueryParams() const
    {
        return this->comp_->hasQueryParams;
    }

    bool Uri::hasFragment() const
    {
        return this->comp_->hasFragment;
    }

    bool Uri::isRelative() const
    {
        return this->comp_->isRelative;
    }



}