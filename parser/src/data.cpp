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

std::string parse_string(std::string s) {
    size_t positions = s.find("Химический состав продукта: ");
    s.erase(0, positions + 52);
    return s;
}

int main() {
    CURL *curl_handle = curl_easy_init();
    std::string html_web_1;
    std::string root = "https://health-diet.ru";
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://health-diet.ru/base_of_meals");
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html_web_1);
        curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
    }
    htmlcxx::HTML::ParserDom parser_web_1;
    tree<htmlcxx::HTML::Node> dom_web_1 = parser_web_1.parseTree(html_web_1);
    for (auto it = dom_web_1.begin(); it != dom_web_1.end(); ++it) {
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
                            std::string html_web_2;
                            CURL *curl_web_2 = curl_easy_init();
                            if (curl_web_2) {
                                it->parseAttributes();
                                curl_easy_setopt(curl_web_2, CURLOPT_URL,
                                                 (root + it->attribute("href").second).c_str());
                                curl_easy_setopt(curl_web_2, CURLOPT_WRITEFUNCTION, writecallback);
                                curl_easy_setopt(curl_web_2, CURLOPT_WRITEDATA, &html_web_2);
                                curl_easy_perform(curl_web_2);
                                curl_easy_cleanup(curl_web_2);
                            }
                            htmlcxx::HTML::ParserDom parser_web_2;
                            tree<htmlcxx::HTML::Node> dom_web_2 = parser_web_2.parseTree(html_web_2);
                            for (auto it_1 = dom_web_2.begin(); it_1 != dom_web_2.end(); ++it_1) {
                                if (it_1->isTag() && it_1->tagName() == "tbody") {
                                    size_t n1 = it_1.number_of_children();
                                    for (size_t j = 0; j < n1; ++j, ++it_1) {
                                        if (it_1->tagName() == "td") {
                                            ++it_1;
                                            while (it_1->tagName() != "a") {
                                                ++it_1;
                                            }
                                            it_1->parseAttributes();
                                            std::cout << parse_string(it_1->attribute("title").second)<< std::endl;
                                            CURL *curl_web_3 = curl_easy_init();
                                            std::string html_web_3;
                                            if (curl_web_3) {
                                                it->parseAttributes();
                                                curl_easy_setopt(curl_web_3, CURLOPT_URL,
                                                                 (root + it_1->attribute("href").second).c_str());
                                                curl_easy_setopt(curl_web_3, CURLOPT_WRITEFUNCTION, writecallback);
                                                curl_easy_setopt(curl_web_3, CURLOPT_WRITEDATA, &html_web_3);
                                                curl_easy_perform(curl_web_3);
                                                curl_easy_cleanup(curl_web_3);
                                            }
                                            htmlcxx::HTML::ParserDom parser_web_3;
                                            tree<htmlcxx::HTML::Node> dom_web_3 = parser_web_3.parseTree(html_web_3);
                                            for (auto it_2 = dom_web_3.begin(); it_2 != dom_web_3.end(); ++it_2) {
                                                if (it_2->isTag()) {
                                                    it_2->parseAttributes();
                                                    if (it_2->tagName() == "table" && it_2->attribute("class").second ==
                                                                                      "mzr-recipe-view-ingredients") {
                                                        while (it_2->tagName() != "tbody") {
                                                            ++it_2;
                                                        }
                                                        size_t n2 = it_2.number_of_children();
                                                        ++it_2;
                                                        for (size_t k = 0; k < n2; ++k, ++it_2) {
                                                            while (it_2->tagName() != "td") {
                                                                ++it_2;
                                                            }
                                                            std::cout << get_child_content(dom_web_3, it_2) << " ";
                                                        }
                                                    }
                                                    if (it_2->attribute("class").second ==
                                                        "mzr-recipe-view-description-tc") {
                                                        std::cout << get_child_content(dom_web_3, it_2) << std::endl;
                                                    }
                                                }
                                            }
                                            for (auto it_2 = dom_web_3.begin(); it_2 != dom_web_3.end(); ++it_2) {
                                                if (it_2->isTag()) {
                                                    it_2->parseAttributes();
                                                    if (it_2->attribute("class").second ==
                                                        "mzr-recipe-view-description-tc") {
                                                        std::cout << get_child_content(dom_web_3, it_2) << std::endl;
                                                    }
                                                }
                                            }
                                            it_1 += 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    it += 2;
                }
            }
        }
    }
}