#include <iostream>
#include <string>
#include "curl/curl.h"
#include <htmlcxx/html/ParserDom.h>
#include <fstream>
#include <cstdio>

#pragma comment(lib, "curllib-bcb.lib")

typedef struct {
    std::string name;
    std::string recipe;
    std::string ingridients;
    std::string img;
} recipe;

typedef struct dish {
    std::string name;
    size_t id{};
    int n;
    double calories{};
    double proteins{};
    double fats{};
    double carbohydrates{};
    recipe *m = new recipe[300000];
} dish;

typedef struct {
    std::string name;
    dish *m = new dish[8];
} category;

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

std::string get_html_string(const std::string &link) {
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

std::string perenos(std::string &s) {
    int a;
    while ((a = s.find('\n')) != std::string::npos) {
        s.erase(s.begin() + a);
    }
    return s;
}

int main() {
    std::string root = "https://yummybook.ru";
    std::string link;
    std::string img = "https:/";
    std::string html_web_1 = get_html_string(root);
    htmlcxx::HTML::ParserDom parser_web_1;
    tree<htmlcxx::HTML::Node> dom_web_1 = parser_web_1.parseTree(html_web_1);
    category food;
    for (auto it = dom_web_1.begin(); it != dom_web_1.end(); ++it) {
        it->parseAttributes();
        if (it->isTag() && it->attribute("class").second == "b-list col-lg-6 col-md-6 col-sm-6") {
            size_t n = it.number_of_children();
            for (size_t i = 0; i < n - 1; ++i, ++it) {
                while (it->tagName() != "img") {
                    ++it;
                }
                it->parseAttributes();
                if (it->attribute("alt").second == "Рецепты для мультиварок" ||
                    it->attribute("alt").second == "Рецепты для детей" ||
                    it->attribute("alt").second == "Выпечка и сладости из яблок" ||
                    it->attribute("alt").second == "Национальные блюда" ||
                    it->attribute("alt").second == "Заготовки, соленья, варенья" ||
                    it->attribute("alt").second == "Соусы" ||
                    it->attribute("alt").second == "Разное" ||
                    it->attribute("alt").second == "Напитки") {
                    while (it->tagName() != "li") {
                        ++it;
                    }
                    continue;
                }
                food.name = it->attribute("alt").second;
                std::ofstream file(food.name);
                std::cout << it->attribute("alt").second << " ";
                while (it->tagName() != "a") {
                    ++it;
                }
                it->parseAttributes();
                std::cout << root + it->attribute("href").second << std::endl;
                std::cout << std::endl;
                std::string html_web_2 = get_html_string((root + it->attribute("href").second));
                htmlcxx::HTML::ParserDom parser_web_2;
                tree<htmlcxx::HTML::Node> dom_web_2 = parser_web_2.parseTree(html_web_2);
                bool next_web = true;
                dish two;
                two.id = 0;
                recipe all;
                while (next_web) {
                    for (auto it_2 = dom_web_2.begin(); it_2 != dom_web_2.end(); ++it_2) {
                        it_2->parseAttributes();
                        if (it_2->isTag() && it_2->attribute("class").second == "b-rec-head") {
                            ++it_2;
                            it_2->parseAttributes();
//                            std::cout << get_child_content(dom_web_2, it_2) << " ";
                            two.name = get_child_content(dom_web_2, it_2);
                            file << two.id << "  " << two.name << "  ";
                            two.id++;
//                            std::cout << root + it_2->attribute("href").second << std::endl;
                            std::string html_web_3 = get_html_string((root + it_2->attribute("href").second));
                            htmlcxx::HTML::ParserDom parser_web_3;
                            tree<htmlcxx::HTML::Node> dom_web_3 = parser_web_3.parseTree(html_web_3);
                            std::string s;
                            for (auto it_3 = dom_web_3.begin(); it_3 != dom_web_3.end(); ++it_3) {
                                it_3->parseAttributes();
                                if (it_3->attribute("class").second == "b-irec-main-pic-image visible-xs-block") {
//                                    std::cout << it_3->attribute("src").second << std::endl;
                                }
                                if (it_3->attribute("class").second == "b-ingredients-list") {
                                    s = "";
                                    size_t n1 = it_3.number_of_children();
                                    for (size_t j = 0; j < n1 - 2; ++j, ++it_3) {
                                        while (it_3->tagName() != "li") {
                                            ++it_3;
                                        }
                                        s += get_child_content(dom_web_3, it_3);
                                    }
                                    all.ingridients = perenos(s);
                                    file << all.ingridients << "  ";
                                }
                            }
                            std::string s1;
                            for (auto it_3 = dom_web_3.begin(); it_3 != dom_web_3.end(); ++it_3) {
                                it_3->parseAttributes();
                                if (it_3->attribute("class").second == "b-irec-stp-item") {
                                    while (it_3->tagName() != "p") {
                                        ++it_3;
                                    }
                                    s1 += get_child_content(dom_web_3, it_3);
//                                    std::cout << get_child_content(dom_web_3, it_3) << std::endl;
                                }
                            }
                            all.recipe = perenos(s1);
                            file << all.recipe << std::endl;
                        }
                    }
                    for (auto it_2 = dom_web_2.begin(); it_2 != dom_web_2.end(); ++it_2) {
                        it_2->parseAttributes();
                        if (it_2->attribute("class").second == "next b-link-pag-next") {
                            ++it_2;
                            link = root + it_2->attribute("href").second;
                        } else if (it_2->attribute("class").second == "next b-link-pag-next disabled") {
                            next_web = false;
                        }
                    }
                    html_web_2 = get_html_string(link);
                    dom_web_2 = parser_web_2.parseTree(html_web_2);
                }
                file.close();
            }
        }
    }
}