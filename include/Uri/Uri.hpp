#ifndef URI_HPP
#define URI_HPP

#include <memory>
#include <string>
#include <vector>
#include <iostream>

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

                bool isValid;

                bool hasUserInfo;

                bool hastHost;

                bool hasPort;

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

            std::string getSchema() const;

            std::string getUserInfo() const;

            std::string getHost() const;

            uint16_t getPort() const;

            std::vector< std::string > getPath() const;

            void extractSchema();

            void extractUserInfo();

            void extractHost();

            void extractPort();




    };




}


#endif