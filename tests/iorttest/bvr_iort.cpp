#include <string>
#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

int main(int argc, char *argv[]) {
    char const* url = "http://127.0.0.1/api/readsonar";
    char const* readsonar = "/api/readsonar";
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.curlpp::Easy::setOpt(curlpp::options::Url(url));
    request.curlpp::Easy::setOpt(curlpp::options::Port(5000));

    std::string effurl;
    curlpp::infos::EffectiveUrl::get(request, effurl);

    std::cout << effurl << std::endl;

    request.perform();
    return 0;
}

