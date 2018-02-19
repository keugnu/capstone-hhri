// LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
// g++ -g --std=c++11 -Werror -I/usr/include bvr_iort.cpp -L/usr/local/lib -lcurlpp -lcurl -o bvr_iort


#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>


#define MAX_FILE_LENGTH 20000


char *m_pBuffer = NULL;
size_t m_Size = 0;


void* Realloc(void* ptr, size_t size)
{
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
};


size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
{
  // Calculate the real size of the incoming buffer
  size_t realsize = size * nmemb;
  
  // (Re)Allocate memory for the buffer
  m_pBuffer = (char*) Realloc(m_pBuffer, m_Size + realsize);
  
  // Test if Buffer is initialized correctly & copy memory
  if (m_pBuffer == NULL) {
    realsize = 0;
  }
  
  memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
  m_Size += realsize;
  
  // return the real size of the buffer...
  return realsize;
};


int rest_req() {
    char const* url = "http://127.0.0.1/api/getcommand";
    char const* url_tts = "http://127.0.0.1/api/gettts";
    
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    std::ostringstream resp;

    request.curlpp::Easy::setOpt(curlpp::options::Url(url));
    request.curlpp::Easy::setOpt(curlpp::options::Port(5000));
    request.curlpp::Easy::setOpt(curlpp::options::Verbose(true)); 
    curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
    curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(functor);

    request.setOpt(test);

    std::string effurl;
    curlpp::infos::EffectiveUrl::get(request, effurl);
    
    request.perform();

    long resp_code; 
    int resp_cmd = (int)m_pBuffer[0];
    curlpp::infos::ResponseCode::get(request, resp_code);
    std::cout << "Response Code: " << resp_code << std::endl << std::endl;

    std::cout << "Size: " << m_Size << std::endl;
    std::cout << "Content: " << std::endl << resp_cmd << std::endl;

    if (resp_cmd == 2) {
        request.curlpp::Easy::setOpt(curlpp::options::Url(url_tts));
        request.perform();
        curlpp::infos::ResponseCode::get(request, resp_code);
        std::cout << "Content: " << std::endl << ++m_pBuffer << std::endl;
    }

    return resp_code;
}


int main(int argc, char *argv[]) {

    m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));

    long resp_code;
    resp_code = rest_req();
    return 0;
}

