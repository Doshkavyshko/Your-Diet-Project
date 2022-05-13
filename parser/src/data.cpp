#include <iostream>
#include <string>
#include "curl/curl.h"
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/Uri.h>

#pragma comment(lib, "curllib-bcb.lib")

size_t writecallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string parse_string(std::string s) {
    size_t positions = s.find("Химический состав продукта: ");
    s.erase(0, positions + 52);
    return s;
}

int main() {
    CURL *curl_handle = curl_easy_init();
    std::string html;
    std::string root = "https://health-diet.ru";
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://health-diet.ru/base_of_meals");
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
    }
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(html);
    for (auto it = dom.begin(); it != dom.end(); ++it) {
        if (it->isTag()) {
            it->parseAttributes();
            if (it->attribute("class").second ==
                "uk-width-1-1 uk-width-small-1-1 uk-width-medium-1-2 uk-width-large-1-2") {
                size_t n = it.number_of_children();
                ++it;
                for (size_t i = 0; i < n; ++i, ++it) {
                    if (it->isTag()) {
                        it->parseAttributes();
                        if (it->attribute("class").second == "uk-flex mzr-tc-group-item") {
                            while (!(it->isTag() && it->tagName() == "a")) {
                                ++it;
                            }
                            it->parseAttributes();
                            std::cout << parse_string(it->attribute("title").second) << " "
                                      << root + it->attribute("href").second << std::endl;
                            it += 2;
                        }
                    }
                }
            }
        }
    }
}
