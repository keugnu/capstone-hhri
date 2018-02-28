// LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
// g++ -g --std=c++11 -Werror -I/usr/include bvr_iort.cpp -L/usr/local/lib -lcurlpp -lcurl -o bvr_iort

#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
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
  m_Size = realsize;
  
  // return the real size of the buffer...
  return realsize;
};


void rest_req(Response &resp, std::string uri) {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    request.curlpp::Easy::setOpt(curlpp::options::Url(uri));
    request.curlpp::Easy::setOpt(curlpp::options::Port(5000));
    curlpp::types::WriteFunctionFunctor functor(WriteMemoryCallback);
    curlpp::options::WriteFunction *writefunc = new curlpp::options::WriteFunction(functor);

    request.setOpt(writefunc);
    request.perform();

    curlpp::infos::ResponseCode::get(request, resp.code);
    resp.size = m_Size;
    std::cout << "In rest_req...\n";
    if (m_Size == 1) { resp.content = m_pBuffer[0]; }
    else { resp.content = ++m_pBuffer; m_pBuffer--; }
    std::cout << "Returning:\n"
 	      << "m_pBuffer[0]: " << +m_pBuffer[0] << std::endl;
/*
    if (m_pBuffer[0] == 2) {
        std::cout << "inside tts request...\n";
        std::cout << "resp.code: " << resp.code << "\t"
                  << "resp.size: " << resp.size << "\t"
                  << "resp.content: " << +m_pBuffer[0] << std::endl; 
        
	request.curlpp::Easy::setOpt(curlpp::options::Url(url_tts));

        std::cout << "m_pBuffer[0] before tts request: " << +m_pBuffer[0] << std::endl;
        request.perform();
        curlpp::infos::ResponseCode::get(request, resp.code);
        resp.content = ++m_pBuffer; m_pBuffer--;
        std::cout << "m_pBuffer after tts request: " << resp.content << std::endl;
	resp.size = m_Size;
        std::cout << "retrieving tts info...\n";
        std::cout << "tts: " << resp.content << std::endl << std::endl;
    }
*/  
}


int main(int argc, char *argv[]) {
    m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
    Response resp;
    char const* get_cmd_uri = "http://127.0.0.1/api/getcommand";
    char const* get_tts_uri = "http://127.0.0.1/api/gettts";

   /* 
    ros::init(argc, argv, "iort");
    ros::NodeHandle n;
    ros::Publisher iort_tts_pub = n.advertise<std_msgs::String>("tts", 10);
    ros::Rate loop_rate(10);
*/
    while(true) {
        m_pBuffer = NULL;
        m_Size = 0;
        resp.content = "";
        resp.size = 0;
        resp.code = 0;
	rest_req(resp, get_cmd_uri);
        std::cout << "In main...\n";
        std::cout << "resp.code: " << resp.code << "\t" 
		  << "resp.size: " << resp.size << "\t"
                  << "resp.content: " << +resp.content[0] << std::endl;

        if (resp.code != 200) {
	    printf("An error with the request has occured.");
        }
        else if (resp.code == 200 && m_pBuffer[0] == 2) {
	    //iort_tts_pub.publish(resp.content);
	    std::cout << "Sending TTS request...\n";
	    rest_req(resp, get_tts_uri);
	    std::cout << "resp.size: " << resp.size << std::endl;
            std::cout << "publishing " << resp.content << std::endl;
	}
 	sleep(3);
    }
    
    return 0;
}

