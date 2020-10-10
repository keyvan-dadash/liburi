#ifndef URI_TESTS
#define URI_TESTS

#include <stdio.h>
#include <Uri/Uri.hpp>
#include <gtest/gtest.h>

TEST(UriTests, ParseAndGetInfoTest1) {
    ASSERT_TRUE(true);
    Uri::Uri uri;
    ASSERT_TRUE(uri.parseFromString("https://google.com/ops?foo=bar&foo1=bar1"));
    ASSERT_EQ(uri.getSchema(), "https");
    ASSERT_EQ(uri.getUserInfo(), "");
    ASSERT_EQ(uri.getHost(), "google.com");
    ASSERT_EQ(uri.getPort(), 443);
    ASSERT_EQ(uri.getPath(), (std::vector< std::string > {
        "ops"
    }));
    ASSERT_EQ(uri.getQueryParams(), (std::map< std::string, std::string > {
        {"foo", "bar"},
        {"foo1", "bar1"}
    }));
}

TEST(UriTests, ParseAndGetInfoTest2) {
    ASSERT_TRUE(true);
    Uri::Uri uri;
    ASSERT_TRUE(uri.parseFromString("https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top"));
    ASSERT_EQ(uri.getSchema(), "https");
    ASSERT_EQ(uri.getUserInfo(), "john.doe");
    ASSERT_EQ(uri.getHost(), "www.example.com");
    ASSERT_EQ(uri.getPort(), 123);
    ASSERT_EQ(uri.getPath(), (std::vector< std::string > {
        "forum",
        "questions",
        ""
    }));
    ASSERT_EQ(uri.getQueryParams(), (std::map< std::string, std::string > {
        {"tag", "networking"},
        {"order", "newest"}
    }));
    ASSERT_EQ(uri.getFragment(), "top");
}

TEST(UriTests, ParseAndGetInfoTest3) {
    ASSERT_TRUE(true);
    Uri::Uri uri;
    ASSERT_TRUE(uri.parseFromString("mailto:John.Doe@example.com"));
    ASSERT_EQ(uri.getSchema(), "mailto");
    ASSERT_EQ(uri.getPath(), (std::vector< std::string > {
        "John.Doe@example.com"
    }));
}


#endif