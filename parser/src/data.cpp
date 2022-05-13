#include "curl/curl.h"
#pragma comment(lib,"curllib-bcb.lib")

int main() {
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://health-diet.ru/base_of_meals");
        CURLcode res = curl_easy_perform(curl_handle);
        curl_easy_cleanup(curl_handle);
    }
    
}
