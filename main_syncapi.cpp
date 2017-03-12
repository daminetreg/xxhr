#include <xxhr/xxhr.hpp>
#include <iostream>

#include <xxhr/apisync.hpp>


int main(int argc, char** argv) {
  xxhr::sync sync_;
  xxhr::GET(xxhr::Url{"CMakeCache.txt"}, xxhr::on_response = sync_);
  auto response = sync_();
  std::cout << "Finally we got the reply : " << response.status_code << " and content : " << response.text << std::endl;
}
