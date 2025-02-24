//
// Created by tank on 2/16/23.
//

#include "utils_for_test.h"
#include "MediaServiceTypes.h"

using namespace media_service;

int main() {
    std::string gateway_addr, gateway_port;
    gateway_addr = utils::GetEnvVariable("LUMINE_GATEWAY_ADDR", "192.168.1.126");
    gateway_port = utils::GetEnvVariable("LUMINE_GATEWAY_PORT", "8082");

    // step 01: connect
    auto request = utils::Socket(gateway_addr.c_str(), std::stoi(gateway_port));
    if (request.conn() < 0) {
        perror("Failed to connect");
        return -1;
    }

    srand(time(nullptr));
    // step 02: make Review
    char *buf = (char *) malloc(1024);

    auto review = reinterpret_cast<Review *>(buf);
    review->review_id = rand();
    review->user_id = rand();
    review->req_id = rand();
    strcpy(review->text, utils::RandomString(64).c_str());
    strcpy(review->movie_id, utils::RandomString(64).c_str());
    review->rating = rand();
    review->timestamp = utils::get_timestamp_us();

    request.issue_http_request("POST", "/function/exp02StoreReview4", buf, sizeof(Review));
    int num_recv = request.recv_response(buf, 1024);

    auto http_parser = new utils::HttpParser(buf, num_recv);
    auto response_payload = http_parser->extract_payload();

    delete http_parser;
    free(buf);
    return 0;
}