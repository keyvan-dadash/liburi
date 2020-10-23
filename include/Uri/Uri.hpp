#ifndef URI_HPP
#define URI_HPP

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <map>

#define DEFAULT_PORT 443

namespace Uri {



    class Uri  {

        private:
            struct uriComponents {

                std::string uri;

                std::string schema;

                std::string userInfo;

                std::string host;

                uint32_t port;

                std::vector< std::string > path;

                std::map< std::string, std::string > queryParams;

                std::string fragment;

                bool isValid;

                bool hasSchema;

                bool hasUserInfo;

                bool hastHost;

                bool hasPort;

                bool hasPath;

                bool hasQueryParams;

                bool hasFragment;

                bool isRelative;

            };

            std::unique_ptr< uriComponents > comp_;

        public:
            ~Uri() noexcept;

            Uri(const Uri& other);

            Uri(Uri&&) noexcept;

            Uri& operator=(const Uri& other);

            Uri& operator=(Uri&&) noexcept;


        public:
            Uri();

            bool parseFromString(std::string uriStirng);

            void resetComponents();

            bool parse();

            void extractSchema();

            void extractUserInfo();

            void extractHost();

            void extractPort();

            void extractPath();

            void extractQueryParams();

            void extractFragment();

            std::string getSchema() const;

            std::string getUserInfo() const;

            std::string getHost() const;

            uint16_t getPort() const;

            std::vector< std::string > getPath() const;

            std::map< std::string, std::string > getQueryParams() const;

            std::string getFragment() const;

            bool isValid() const;

            bool hasSchema() const;

            bool hasUserInfo() const;

            bool hasHost() const;

            bool hasPort() const;

            bool hasPath() const;

            bool hasQueryParams() const;

            bool hasFragment() const;

            bool isRelative() const;

    };




}

namespace UriUtils {

    std::vector< std::string > SplitUriPath(std::string path);

    void validateSchema(std::string schema);
    
}


#endif