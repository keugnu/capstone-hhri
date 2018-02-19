// LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
// g++ -g --std=c++11 -Werror -I/usr/include bvr_iort.cpp -L/usr/local/lib -lcurlpp -lcurl -o bvr_iort

#include <unistd.h>
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


class Response {
    public:
        long code;
        int size;
        std::string content;
};

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


void rest_req(Response &resp) {
    char const* url = "http://127.0.0.1/api/getcommand";
    char const* url_tts = "http://127.0.0.1/api/gettts";
    
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.curlpp::Easy::setOpt(curlpp::options::Url(url));
    request.curlpp::Easy::setOpt(curlpp::options::Port(5000));
    curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
    curlpp::options::WriteFunction *writefunc = new curlpp::options::WriteFunction(functor);

    request.setOpt(writefunc);
    request.perform();

    curlpp::infos::ResponseCode::get(request, resp.code);
    resp.content = m_pBuffer;
    resp.size = m_Size;
    if (resp.code == 5) {
        request.curlpp::Easy::setOpt(curlpp::options::Url(url_tts));
        request.perform();
        curlpp::infos::ResponseCode::get(request, resp.code);
    }
}


int main(int argc, char *argv[]) {
    m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
    Response resp;
   /* 
    ros::init(argc, argv, "iort");
    ros::NodeHandle n;
    ros::Publisher iort_tts_pub = n.advertise<std_msgs::String>("tts", 10);
    ros::Rate loop_rate(10);
*/
    while(true) {
	rest_req(resp);

        if (resp.code != 200) {
	    printf("An error with the request has occured.");
        }
        else if (resp.code == 200 && m_Size > 1) {
	    //iort_tts_pub.publish(++m_pBuffer);
	    printf("publishing %s", m_pBuffer);
	}
 	sleep(10);
    }
    
    return 0;
}

