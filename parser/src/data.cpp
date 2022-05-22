#include <iostream>
#include <string>
#include "curl/curl.h"
#include <htmlcxx/html/ParserDom.h>
#include <htmlcxx/html/Uri.h>
#include <clocale>

#pragma comment(lib, "curllib-bcb.lib")

size_t writecallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}


std::string get_child_content(tree<htmlcxx::HTML::Node> const &dom, tree<htmlcxx::HTML::Node>::iterator const &parent) {
    std::string result;
    for (unsigned i = 0; i < dom.number_of_children(parent); ++i) {
        tree<htmlcxx::HTML::Node>::iterator it = dom.child(parent, i);
        if (!it->isTag() && !it->isComment()) {
            result += it->text();
        }
    }
    return result;
}

void get_char(std::string &s) {
    for (int i = 0; i < 10; ++i) {
        std::cout << int(s[i]) << " ";
    }
    std::cout << std::endl;
}

std::string get_html_string(const std::string& link) {
    CURL *curl_handle = curl_easy_init();
    std::string html;
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, link.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
    }
    return html;
}

int main() {
    std::string root = "https://www.iamcook.ru";
    std::string link = "https://www.iamcook.ru/event/everyday";
    std::string html_web_1 = get_html_string(link);
    htmlcxx::HTML::ParserDom parser_web_1;
    tree<htmlcxx::HTML::Node> dom_web_1 = parser_web_1.parseTree(html_web_1);
    for (auto it = dom_web_1.begin(); it != dom_web_1.end(); ++it) {
        it->parseAttributes();
        if(it->isTag() && it->attribute("class").second == "themelinks") {
            size_t n = it.number_of_children();
            ++it;
            for (size_t i = 0; i < n; ++i, ++it) {
                while(it->isTag() && it->tagName() != "a") {
                    ++it;
                }
                it->parseAttributes();
                std::string html_web_2 = get_html_string(root + it->attribute("href").second);
                htmlcxx::HTML::ParserDom parser_web_2;
                tree<htmlcxx::HTML::Node> dom_web_2 = parser_web_2.parseTree(html_web_2);
                while(true) {
                    for (auto it_2 = dom_web_2.begin(); it_2 != dom_web_2.end(); ++it_2) {
                        it_2->parseAttributes();
                        if (it_2->isTag() && it_2->attribute("class").second == "recfield") {
                            size_t n2 = it_2.number_of_children();
                            ++it_2;
                            for (size_t j = 0; j < n2; ++j, ++it_2) {
                                while (it_2->tagName() != "a") {
                                    ++it_2;
                                }
                                ++it_2;
                                if (it_2->tagName() == "img") {
                                    ++it_2;
                                    while (it_2->tagName() != "a") {
                                        ++it_2;
                                    }
                                    it_2->parseAttributes();
                                    std::cout << get_child_content(dom_web_2, it_2) << std::endl;
                                }
                            }
                        }
                    }
                    bool page = false;
                    for (auto it_2 = dom_web_2.begin(); it_2 != dom_web_2.end(); ++it_2) {
                        it_2->parseAttributes();
                        if (it_2->attribute("class").second == "navpage") {
                            html_web_2 = get_html_string(root + it_2->attribute("href").second);
                            page = true;
                        }
                    }
                    if (!page) {
                        break;
                    }
                    dom_web_2 = parser_web_2.parseTree(html_web_2);
                }
                it += 3;
                break;
            }
        }
    }
}