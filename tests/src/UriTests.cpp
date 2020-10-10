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
        "",
        "ops"
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
        "",
        "ops"
    }));
}

#endif